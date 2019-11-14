#include "lexer.hpp"
#include "optimise.tcc"
#include <cassert>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <type_traits>

#include "unicode/class.hpp"

inline static void lexer_error_impl(char const *fmt, va_list arg) {
  std::vprintf(fmt, arg);
}

void lexer_error(const NLexer &lexer, int errn, const Token &tok,
                 ErrorPosition pos, char const *fmt...) {
  char buf[1024];
  std::sprintf(buf, "[E%04d] %s - (line %d offset %d length %d) - %s\n", errn,
               lexer.errors[errn], tok.lineno, tok.offset, tok.length, fmt);
  va_list arg;
  va_start(arg, fmt);
  lexer_error_impl(buf, arg);
  va_end(arg);
}

inline static const int utf8_encode(char *out, uint32_t utf) {
  if (utf <= 0x7F) {
    out[0] = (char)utf;
    out[1] = 0;
    return 1;
  } else if (utf <= 0x07FF) {
    out[0] = (char)(((utf >> 6) & 0x1F) | 0xC0);
    out[1] = (char)(((utf >> 0) & 0x3F) | 0x80);
    out[2] = 0;
    return 2;
  } else if (utf <= 0xFFFF) {
    out[0] = (char)(((utf >> 12) & 0x0F) | 0xE0);
    out[1] = (char)(((utf >> 6) & 0x3F) | 0x80);
    out[2] = (char)(((utf >> 0) & 0x3F) | 0x80);
    out[3] = 0;
    return 3;
  } else if (utf <= 0x10FFFF) {
    out[0] = (char)(((utf >> 18) & 0x07) | 0xF0);
    out[1] = (char)(((utf >> 12) & 0x3F) | 0x80);
    out[2] = (char)(((utf >> 6) & 0x3F) | 0x80);
    out[3] = (char)(((utf >> 0) & 0x3F) | 0x80);
    out[4] = 0;
    return 4;
  } else {
    out[0] = (char)0xEF;
    out[1] = (char)0xBF;
    out[2] = (char)0xBD;
    out[3] = 0;
    return 0;
  }
}

inline static CUDebugInformation
regexp_debug_info(NLexer *lexer, std::string name, int length) {
  return {lexer->lineno, lexer->offset - length, length, name};
}

char const *NLexer::errors[Errors::LAST] = {
    [Errors::Unexpected] = "Unexpected Token",
    [Errors::ExpectedValue] = "Expected a value",
    [Errors::InvalidRegexp] = "Invalid Regexp",
    [Errors::RegexpExpectedSymbolExpression] = "Expected a symbol expression",
    [Errors::RegexpInvalidSymbolExpression] = "Invalid symbol expression",
    [Errors::RegexpUnsupported] = "Unsupported Regex",
    [Errors::RegexpUnknown] = "Unknown regex",
    [Errors::RegexpQuantifierRepeatInvalidSyntax] =
        "Invalid Repeat quantifier syntax",
    [Errors::InvalidRegexpSyntax] = "Invalid regex syntax",
    [Errors::InvalidCodepointSpecification] = "Invalid Codepoint value",
    [Errors::UnknownUnicodeClassName] = "Unknown Unicode Class name",
};

void Token::print() {
  if (type == TokenType::TOK_BOOL)
    std::printf("Token { %s, line %d, offset %d, length %d, value = <%s> }\n",
                reverse_token_type[type], lineno, offset, length,
                std::get<bool>(value) ? "true" : "false");
  else if (type == TokenType::TOK_REGEX)
    std::printf("Token { %s, line %d, offset %d, length %d, value = <%s> }\n",
                reverse_token_type[type], lineno, offset, length,
                std::get<Regexp>(value).to_str().c_str());
  else
    std::printf("Token { %s, line %d, offset %d, length %d, value = <%s> }\n",
                reverse_token_type[type], lineno, offset, length,
                std::get<std::string>(value).c_str());
}

char escapes[256] = {
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\\', '\0', '\0', '\0', '\0',
    '\a', '\b', '\0', '\0', '\e', '\f', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\n', '\0', '\0', '\0', '\r', '\0', '\t', '\0', '\v', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0',
};

void NLexer::advance(int i) {
  source_p += i;
  offset += i;
}

Token NLexer::error_token() {
  return {TOK_ERROR, lineno, offset, 0, empty_string};
}

inline void NLexer::comment(char c) {
  if (c == '#') {
    do {
      c = *source_p;
      if (!c)
        return;
      advance(1);
      if (c == '\n') {
        lineno++;
        offset = 0;
        break;
      }
    } while (1);
  }
}

const Token NLexer::next() {
  const Token tok = _next();
  prev_token = tok;
  return tok;
}

inline Token NLexer::_next() {
  char c;
  int length = -1;
  int commentl = false;
  if (!*source_p)
    return Token{TOK_EOF, lineno, offset, 0};
  do {
    if (!*source_p)
      return Token{TOK_EOF, lineno, offset, 0};
    c = *(source_p++);
    offset++;
    if (c == '#') {
      commentl = true;
      continue;
    }
    if (c == '\n') {
      lineno++;
      offset = 0;
      commentl = false;
      continue;
    }

    if (isspace(c) || commentl)
      continue;
    break;
  } while (1);

  switch (state) {
  case LexerState::Toplevel: {
    if (c == 'o' && strncmp("ption", source_p, strlen("ption")) == 0 &&
        isspace(*(source_p + strlen("ption")))) {
      state = LexerState::Option;
      advance(strlen("ption"));
      return Token{TOK_OPTION, lineno, offset - strlen("option"),
                   strlen("option"), empty_string};
    }

    if (c == 's' && strncmp("topword", source_p, strlen("topword")) == 0 &&
        !isalnum(*(source_p + strlen("topword")))) {
      state = LexerState::Stopword;
      advance(strlen("topword"));
      return Token{TOK_STOPWORD, lineno, offset - strlen("stopword"),
                   strlen("stopword"), empty_string};
    }
    if (c == 'i' && strncmp("gnore", source_p, strlen("gnore")) == 0 &&
        !isalnum(*(source_p + strlen("gnore")))) {
      state = LexerState::Ignore;
      advance(strlen("gnore"));
      return Token{TOK_IGNORE, lineno, offset - strlen("ignore"),
                   strlen("ignore"), empty_string};
    }
    if (c == 'n' && strncmp("ormalise", source_p, strlen("ormalise")) == 0 &&
        !isalnum(*(source_p + strlen("ormalise")))) {
      state = LexerState::NormalSrc;
      advance(strlen("ormalise"));
      return Token{TOK_OPNORMAL, lineno, offset - strlen("normalise"),
                   strlen("normalise"), empty_string};
    }
    length = 0;
    buffer[length++] = c;
    do {
      c = *(source_p++);
      if (!c)
        break;
      buffer[length++] = c;
      offset++;
    } while (!isspace(c));
    buffer[length--] = 0;
    offset--;
    if (length == 0) {
      if (!*source_p)
        return Token{TOK_EOF, lineno, offset, 0};
      return Token{TOK_EOF, lineno, offset, 0};
      // ...
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::ExpectedValue, mtoken, ErrorPosition::After,
                  "Expected an identifier");
      return mtoken;
    }

    state = LexerState::Name;

    return Token{TOK_NAME, lineno, offset - length, length,
                 std::string{buffer, length}};
    break;
  }
  case LexerState::Name: {
    const Token &mtoken = error_token();
    // if (codepoints::is_single_code_point(
    //         std::get<std::string>(prev_token.value)) &&
    //     c == '<') {
    //   // norm
    //   if (strncmp("=", source_p, 1) != 0) {
    //     lexer_error(*this, Errors::Unexpected, mtoken, ErrorPosition::On,
    //                 "Expected `<='");
    //     return mtoken;
    //   }
    //   advance(1);
    //   state = LexerState::Normal;
    //   return Token{TOK_OPNORMAL, lineno, offset, 2, empty_string};
    // }
    if (c != ':' && c != '-') {
      lexer_error(*this, Errors::Unexpected, mtoken, ErrorPosition::On,
                  "Expected `::', `:-' or `--'");
      return mtoken;
    }
    if (c == ':') {
      if (strncmp(":", source_p, 1) == 0) {
        state = LexerState::Define;
        advance(1);
        return Token{TOK_OPDEFINE, lineno, offset, 2, empty_string};
      }

      if (strncmp("-", source_p, 1) == 0) {
        state = LexerState::Const;
        advance(1);
        return Token{TOK_OPCONST, lineno, offset, 2, empty_string};
      }
      lexer_error(*this, Errors::Unexpected, mtoken, ErrorPosition::After,
                  "Expected either `::' or `:-'");
      return mtoken;
    } else {
      if (strncmp("-", source_p, 1) == 0) {
        state = LexerState::Literal;
        advance(1);
        return Token{TOK_OPLIT, lineno, offset, 2, empty_string};
      }
      lexer_error(*this, Errors::Unexpected, mtoken, ErrorPosition::After,
                  "Expected `--'");
      return mtoken;
    }
    break;
  }
  case LexerState::Option: {
    buffer[++length] = c;
    do {
      c = *(source_p++);
      buffer[++length] = c;
      offset++;
    } while (!isspace(c));
    buffer[length] = 0;
    offset--;

    state = LexerState::OptionBool;

    return Token{TOK_NAME, lineno, offset - length, length,
                 std::string{buffer, length}};
    break;
  }
  case LexerState::OptionBool: {
    bool truth;
    if (c == 'o' && strncmp("ff", source_p, 2) == 0 &&
        isspace(*(source_p + 2))) {
      advance(2);
      length = 3;
      truth = false;
    } else if (c == 'o' && strncmp("n", source_p, 1) == 0 &&
               (!*(source_p + 1) || isspace(*(source_p + 1)))) {
      advance(1);
      length = 2;
      truth = true;
    } else {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::Unexpected, mtoken, ErrorPosition::On,
                  "Expected either `on' or `off'");
      return mtoken;
    }

    state = LexerState::Toplevel;

    return Token{TOK_BOOL, lineno, offset - length, length, truth};
    break;
  }
  case LexerState::Ignore: {
    if (c != '[') {

      const Token &mtoken = error_token();
      lexer_error(*this, Errors::Unexpected, mtoken, ErrorPosition::On,
                  "Expected an open bracket '['");
      return mtoken;
    }
    state = LexerState::IgnoreBrac;
    return next();
  }
  case LexerState::IgnoreBrac: {
    if (c == ']') {
      state = LexerState::Toplevel;
      return Token{TOK_CBRAC, lineno, offset - 1, 1, std::string{"]"}};
    }
    buffer[++length] = c;
    do {
      c = *(source_p++);
      buffer[++length] = c;
      offset++;
    } while (!isspace(c) && c != ']');

    buffer[length] = 0;
    offset--;
    if (c == ']')
      source_p--;

    return Token{TOK_NAME, lineno, offset - length, length,
                 std::string{buffer, length}};
    break;
  }
  case LexerState::Literal: {
    if (c != '-' && c != '"') {
      advance(-1);
      state = LexerState::Toplevel;
      return next();
    }
    bool isfile = false;
    if (c == '-')
      isfile = true;
    else if (c == '"')
      source_p--;
    std::optional<std::string> vstr = string(false);
    if (!vstr.has_value()) {
      state = LexerState::Toplevel;
      return {TOK_ERROR, lineno, offset, 0, empty_string};
    }
    std::string str = vstr.value();
    return Token{isfile ? TOK_FILESTRING : TOK_LITSTRING, lineno,
                 offset - str.size(), str.size(), str};
  }
  case LexerState::Stopword: {
    if (c != '-' && c != '"') {
      source_p--;
      offset--;
      state = LexerState::Toplevel;
      return next();
    }
    bool isfile = false;
    if (c == '-')
      isfile = true;
    else if (c == '"')
      source_p--;
    std::optional<std::string> vstr = string();
    if (!vstr.has_value()) {
      state = LexerState::Toplevel;
      return {TOK_ERROR, lineno, offset, 0, empty_string};
    }
    std::string str = vstr.value();
    return Token{isfile ? TOK_FILESTRING : TOK_LITSTRING, lineno,
                 offset - str.size(), str.size(), str};
  }
  case LexerState::Const: {
    bool isfile = false;
    if (c == '-')
      isfile = true;
    else if (c == '"')
      source_p--;
    else {
      advance(-1); // attempt to parse regex
      Token errtok = error_token();
      auto opt = _regexp();
      if (!opt.has_value()) {
        const Token &mtoken = error_token();
        lexer_error(
            *this, Errors::InvalidRegexp, mtoken, ErrorPosition::On,
            "Expected a valid regex (see above for possible diagnosis)");
        return mtoken;
      }
      Regexp regex = opt.value();
      if (!regex.sanity_check(*this)) {
        errtok.length = regex.str.size();
        lexer_error(*this, Errors::InvalidRegexp, errtok, ErrorPosition::After,
                    "Malformed Regular expression");
        return errtok;
      }
      state = LexerState::Toplevel;
      return Token{TOK_REGEX, lineno, offset - regex.str.size(),
                   regex.str.size(), regex};
    }
    std::optional<std::string> vstr = string();
    state = LexerState::Toplevel;
    if (!vstr.has_value()) {
      state = LexerState::Toplevel;
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::ExpectedValue, mtoken, ErrorPosition::After,
                  "Expected a string");
      return {TOK_ERROR, lineno, offset, 0, empty_string};
    }
    std::string str = vstr.value();
    return Token{isfile ? TOK_FILESTRING : TOK_LITSTRING, lineno,
                 offset - str.size(), str.size(), str};
  }
  case LexerState::NormalSrc: {
    length = 0;
    bool escape = false;
    if (c != '{') {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::InvalidCodepointSpecification, mtoken,
                  ErrorPosition::On,
                  "Expected normalisation source to be in braces (e.g. "
                  "'normalise {abc} ...')");
    }
    c = *source_p;
    advance(1);
    while (1) {
      if (c == '\\') {
        if (!escape) {
          escape = true;
          if ((c = *source_p) == '+') {
            // \+HHHH(HH)?
            // 4-chars seem to be the common format for smaller ones too
            int len = 0;
            uint32_t codepoint = 0;
            advance(1);
            c = *source_p;
            while ((c <= '9' && c >= '0') || (c <= 'F' && c >= 'A') ||
                   (c <= 'f' && c >= 'a')) {
              if (len++ == 6)
                break;
              if (c <= '9')
                codepoint = codepoint * 16 + (c - '0');
              else if (c <= 'F')
                codepoint = codepoint * 16 + (c - 'A' + 10);
              else
                codepoint = codepoint * 16 + (c - 'a' + 10);
              advance(1);
              c = *source_p;
            }
            if (len < 4 && len > 6) {
              const Token &mtoken = error_token();
              lexer_error(*this, Errors::InvalidCodepointSpecification, mtoken,
                          ErrorPosition::On,
                          "Expected between 4 and 6 hexadecimal characters to "
                          "follow \\+, not %d",
                          len);
            }
            length += utf8_encode(buffer + length, codepoint);
            escape = false;
            c = *source_p;
            continue;
          }
        }
      }
      if (c == '}') {
        if (escape)
          escape = false;
        else
          break;
      }
      if (c == ' ') {
        if (escape)
          escape = false;
        else {
          c = *source_p;
          advance(1);
          continue;
        }
      }
      if (c == '\0' || c == '\n')
        break;
      buffer[length++] = c;
      c = *source_p;
      advance(1);
    }
    buffer[length] = 0;
    state = LexerState::NormalTo;
    return Token{TOK_LITSTRING, lineno, offset - length, length,
                 std::string{buffer, length}};
  }
  case LexerState::NormalTgt: {
    length = 0;
    bool escape = false;
    while (1) {
      if (c == '\\') {
        if (!escape) {
          escape = true;
          if ((c = *source_p) == '+') {
            // \+HHHH(HH)?
            // 4-chars seem to be the common format for smaller ones too
            int len = 0;
            uint32_t codepoint = 0;
            advance(1);
            c = *source_p;
            while ((c <= '9' && c >= '0') || (c <= 'F' && c >= 'A') ||
                   (c <= 'f' && c >= 'a')) {
              if (len++ == 6)
                break;
              if (c <= '9')
                codepoint = codepoint * 16 + (c - '0');
              else if (c <= 'F')
                codepoint = codepoint * 16 + (c - 'A' + 10);
              else
                codepoint = codepoint * 16 + (c - 'a' + 10);
              advance(1);
              c = *source_p;
            }
            if (len < 4 && len > 6) {
              const Token &mtoken = error_token();
              lexer_error(*this, Errors::InvalidCodepointSpecification, mtoken,
                          ErrorPosition::On,
                          "Expected between 4 and 6 hexadecimal characters to "
                          "follow \\+, not %d",
                          len);
            }
            length += utf8_encode(buffer + length, codepoint);
            escape = false;
            c = *source_p;
            continue;
          }
        }
      }
      if (c == '\0' || c == '\n')
        break;
      buffer[length++] = c;
      c = *source_p;
      advance(1);
    }
    if (c == 0)
      advance(-1);
    buffer[length] = 0;
    if (!codepoints::is_single_code_point(buffer)) {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::ExpectedValue, mtoken, ErrorPosition::On,
                  "Normalisation target can only be a single unicode codepoint "
                  "('%s' has %d characters, which should've been %d)",
                  buffer, length, Codepoints::getlength(buffer));
      // let it be anyway
      length = Codepoints::getlength(buffer);
      buffer[length] = 0; // pick the first codepoint
    }
    state = LexerState::Toplevel;
    return Token{TOK_LITSTRING, lineno, offset - length, length,
                 std::string{buffer, length}};
  }
  case LexerState::NormalTo: {
    if (c == 't' && strncmp("o", source_p, strlen("o")) == 0 &&
        !isalnum(*(source_p + strlen("o")))) {
      state = LexerState::NormalTgt;
      advance(1);
      return _next();
    }

    const Token &mtoken = error_token();
    lexer_error(*this, Errors::ExpectedValue, mtoken, ErrorPosition::On,
                "Expected 'to' after the first section of normalise");
    return mtoken;
  }
  case LexerState::Define: {
    // parse a regexp
    advance(-1);
    Token errtok = error_token();
    auto opt = _regexp();
    if (!opt.has_value()) {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::InvalidRegexp, mtoken, ErrorPosition::On,
                  "Expected a valid regex (see above for possible diagnosis)");
      return mtoken;
    }
    Regexp regex = opt.value();
    if (!regex.sanity_check(*this)) {
      errtok.length = regex.str.size();
      lexer_error(*this, Errors::InvalidRegexp, errtok, ErrorPosition::After,
                  "Malformed Regular expression");
      return errtok;
    }
    state = LexerState::Toplevel;
    return Token{TOK_REGEX, lineno, offset - regex.str.size(), regex.str.size(),
                 regex};
  }
  default:
    /* code */
    break;
  }
}

std::optional<Regexp> NLexer::_regexp() {
  // cleanup leading spaces
  do {
    char c = *source_p;
    advance(1);
    if (isspace(c))
      continue;
    break;
  } while (1);
  advance(-1);
  return regexp();
}

/*
       (                                         # subpattern 1
        (?:                                      # main body*
            (?:                                  # expression = {{{
               [^?+*{}()[\]\\|]+                 # any series of literals
              | \\.                              # escape
              | \[                               # character class = [[[
                  (?:                            # cc body 0
                       \^?\\.                    # either "^escape"
                     | \^[^\\]                   # or ^[anything not an escape]
                     |  [^\\^]                   # something that's not a
   backslash or caret
                  )
                  (?:                            # what even are you?
                       [^\]\\]+                  # any number of not ] or
   backslash | \\.                       # or an escape
                  )*                             # repeated any number of times
                \]                               # ]]]
              | \(                               # parenthesised expression? (((
                  (?:                            # paren attribute
                       \?[:=!]                   # ?: | ?= | ?!
                     | \?<[=!]                   # ?<= | ?<!
                     | \?>                       # ?>
                  )?                             # optionally
                  (?1)??                         # some regex
                \)                               # )))
              | \(                               # recursion
                  \?                             # ?...
                  (?:                            # ?R | ?1... | relative ?-1
                       R                         #
                     | [+-]?\d+                  #
                  )                              #
                \)                               # recursion end
            )                                    # expression }}}
            (?:                                  # optionally a quantifier
                (?:                              #
                     [?+*]                       # lazy | plus | kleene
                   | \{                          # {1} | {1,2}
                       \d+
                       (?:,\d*)?
                     \}
                )
                [?+]?                            # lazy or plus attribute for
   lazy?
            )?                                   # optionally
          | \|                                   # or a union
          | \{\{\w+\}\}                          # NLexer addition
        )*                                       # repeated indefinitely
       )
*/

// subpattern 1
std::optional<Regexp> NLexer::regexp() {
  std::optional<Regexp> reg = {}, reg1 = {};
  do {
    reg = regexp_tl();
    if (!reg.has_value())
      break;
    if (reg1.has_value())
      reg1 = reg1->concat(reg.value());
    else
      reg1 = reg.value();
  } while (1);
  return reg1;
}

// main body
std::optional<Regexp> NLexer::regexp_tl() {
  // either parse an expression followed by quantifiers,
  // or parse a pipe followed by some regex
  // or parse a {{symbol}}
  char c = *source_p;
  if (c == '\n')
    return {};
  if (c == '{') {
    // must be a {{symbol}}
    if (!*(source_p + 1) || *(source_p + 1) != '{') {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::RegexpExpectedSymbolExpression, mtoken,
                  ErrorPosition::After, "Expected `{{' followed by a symbol");
      return {};
    }
    // it starts correctly
    int length = 0;
    advance(2);
    while (1) {
      c = *source_p;
      advance(1);
      if (!isalnum(c))
        break;
      buffer[length++] = c;
    }
    advance(-1);
    if (strncmp("}}", source_p, 2) != 0) {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::RegexpInvalidSymbolExpression, mtoken,
                  ErrorPosition::After, "Expected `}}' to follow a symbol");
      return {};
    }
    advance(2);
    auto value = std::string{buffer, length};
    return Regexp{std::string{source_p - length - 4, length + 4},
                  RegexpType::Symbol, value,
                  regexp_debug_info(this, value, length)};
  }
  if (c == '|') {
    // is pipe, generate alternative
    advance(1);
    auto alt = regexp();
    if (!alt.has_value())
      return alt;
    std::vector<Regexp *> vec;
    vec.push_back(new Regexp{alt.value()});
    return Regexp{alt.value().str, RegexpType::Alternative, vec,
                  regexp_debug_info(this, "|", alt.value().str.size() + 1)};
  }
  // otherwise parse an expression
  auto exp = regexp_expression();
  if (!exp.has_value())
    return exp;
  return regexp_quantifier(exp.value());
}

std::optional<Regexp> NLexer::regexp_expression() {
  char c = *source_p;
  if (c == '\0')
    return {};
  // character literal
  if (strchr("?+*{}()[]\\|.^$", c) == NULL) {
    advance(1);
    return Regexp{std::string{source_p - 2, 1}, RegexpType::Literal, c,
                  regexp_debug_info(this, std::string{c}, 1)};
  }
  if (c == '.') {
    // match anything
    advance(1);
    return Regexp{std::string{source_p - 1, 1}, RegexpType::CharacterClass,
                  "]\n", regexp_debug_info(this, std::string{'.'}, 1)};
  }
  // escaped character
  if (c == '\\') {
    advance(1);
    c = *source_p;
    advance(1);
    if (isdigit(c)) {
      // backref ?
      static char buf[40];
      int len = 0;
      while (isdigit(c)) {
        buf[len++] = c;
        advance(1);
        c = *source_p;
      }
      buf[len] = 0;
      int backrefnum = atoi(buf);
      advance(-1);
      if (backrefnum > nested_index) {
        lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                    ErrorPosition::On,
                    "Backreference references future or current match (%d>%d)",
                    backrefnum, nested_index);
        backrefnum = nested_index - 2;
      }
      auto reg = Regexp{std::string{source_p - len, len}, RegexpType::Escape,
                        '\\', regexp_debug_info(this, std::string{'\\'}, 1)};
      reg.index = backrefnum;
      return reg;
    }
    if (strchr("?+*{}()[]\\|.^$", c) != NULL)
      // switch to literal
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Literal, c,
                    regexp_debug_info(this, std::string{c}, 1)};

    // magic escapes
    switch (c) {
    case 's': // whitespace
      return Regexp{std::string{source_p - 2, 2}, RegexpType::CharacterClass,
                    (std::string)UnicodeClasses::all_of_class(
                        UnicodeClasses::Whitespaces),
                    regexp_debug_info(this, "\\s", 2)};
    case 'S': // whitespace
      return Regexp{std::string{source_p - 2, 2}, RegexpType::CharacterClass,
                    "]" + (std::string)UnicodeClasses::all_of_class(
                              UnicodeClasses::Whitespaces),
                    regexp_debug_info(this, "\\S", 2)};
    case 'w':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::CharacterClass,
                    (std::string)UnicodeClasses::Words,
                    regexp_debug_info(this, "\\w", 2)};
    case 'W':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::CharacterClass,
                    "]" + (std::string)UnicodeClasses::all_of_class(
                              UnicodeClasses::Words),
                    regexp_debug_info(this, "\\W", 2)};
    case 'd':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::CharacterClass,
                    (std::string)UnicodeClasses::Digits,
                    regexp_debug_info(this, "\\d", 2)};
    case 'D':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::CharacterClass,
                    "]" + (std::string)UnicodeClasses::all_of_class(
                              UnicodeClasses::Digits),
                    regexp_debug_info(this, "\\D", 2)};
    case '0':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Literal, '\0',
                    regexp_debug_info(this, "\\0", 2)};
    case 'n':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Literal, '\n',
                    regexp_debug_info(this, "\\n", 2)};
    case 'f':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Literal, '\f',
                    regexp_debug_info(this, "\\f", 2)};
    case 'r':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Literal, '\r',
                    regexp_debug_info(this, "\\r", 2)};
    case 't':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Literal, '\t',
                    regexp_debug_info(this, "\\t", 2)};
    case 'v':
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Literal, '\v',
                    regexp_debug_info(this, "\\v", 2)};
    case 'p':
    case 'P': {
      char pc = *source_p;
      if (pc != '{')
        lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                    ErrorPosition::On,
                    "Expected an open-brace '{' to follow \\p or \\P");
      else
        advance(1); // attempt to read charclass
      // charclass can have two or one chars
      char bf[3] = {0}, *bufv = &bf[0];
      pc = *source_p;
      if (strchr("LMNPSZC", pc) == NULL)
        lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                    ErrorPosition::On,
                    "Invalid unicode character class, expected any of LMNPSZC "
                    "but got %c",
                    pc);
      *(bufv++) = pc;
      advance(1);
      pc = *source_p;
      if (pc == '}') {
        advance(1);
        return Regexp{std::string{source_p - 5, 5}, RegexpType::CharacterClass,
                      (c == 'P' ? "]" : "") +
                          (std::string)UnicodeClasses::all_of_class(bf),
                      regexp_debug_info(this, std::string{source_p - 5, 5}, 5)};
      }
      *(bufv++) = pc;
      advance(1);
      pc = *source_p;
      if (pc == '}') {
        advance(1);
        return Regexp{std::string{source_p - 6, 6}, RegexpType::CharacterClass,
                      (c == 'P' ? "]" : "") +
                          (std::string)UnicodeClasses::all_of_class(bf),
                      regexp_debug_info(this, std::string{source_p - 6, 6}, 6)};
      } else {
        lexer_error(
            *this, Errors::InvalidRegexpSyntax, error_token(),
            ErrorPosition::On,
            "Expected a close-brace '}' to follow \\p or \\P expression");
        return Regexp{std::string{source_p - 6, 6}, RegexpType::CharacterClass,
                      (c == 'P' ? "]" : "") +
                          (std::string)UnicodeClasses::all_of_class(bf),
                      regexp_debug_info(this, std::string{source_p - 6, 6}, 6)};
      }
    }
    case 'K':
      return Regexp{std::string{source_p - 2, 2},
                    RegexpType::Assertion,
                    {RegexpAssertion::SetPosition},
                    regexp_debug_info(this, "\\K", 2)};
    case 'A':
      return Regexp{std::string{source_p - 2, 2},
                    RegexpType::Assertion,
                    {RegexpAssertion::TrueBeginning},
                    regexp_debug_info(this, "\\A", 2)};
    case 'G':
      return Regexp{std::string{source_p - 2, 2},
                    RegexpType::Assertion,
                    {RegexpAssertion::MatchBeginning},
                    regexp_debug_info(this, "\\G", 2)};
    case 'g': {
      c = *source_p;
      advance(1);
      // advance(1);
      if (isdigit(c)) {
        // subexpr call
        static char buf[40];
        int len = 0;
        while (isdigit(c)) {
          buf[len++] = c;
          advance(1);
          c = *source_p;
        }
        buf[len] = 0;
        int backrefnum = atoi(buf);
        advance(-1);
        if (backrefnum > nested_index) {
          lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                      ErrorPosition::On,
                      "Subexpr call references future or current match (%d>%d)",
                      backrefnum, nested_index);
          backrefnum = -1;
        }
        auto reg = Regexp{std::string{source_p - len - 1, len + 1},
                          RegexpType::SubExprCall, (char)backrefnum,
                          regexp_debug_info(this, "\\g", 2)};
        reg.subexprcall = backrefnum;
        return reg;
      }
      lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                  ErrorPosition::On,
                  "Subexpr call (\\g) expects a number (\\g<n>) not '%c'", c);
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Literal, 'g',
                    regexp_debug_info(this, "\\g", 2)};
    }
    case 'u': {
      static char buf[7] = {0};
      int len = 0;
      c = *source_p;
      advance(1);
      c = tolower(c);
      while (isxdigit(c)) {
        if (len == 6)
          break;
        buf[len++] = c;
        advance(1);
        c = *source_p;
        c = tolower(c);
      }
      if (len < 3) {
        lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                    ErrorPosition::On,
                    "Expected 4 to 6 hex digits after \\u, not %d", len);
      }
      uint32_t cp = strtol(buf, NULL, 16);
      char bufv[5];
      int ll = utf8_encode(bufv, cp);
      return Regexp{
          std::string{source_p - len - 4, len + 4}, // single-char charclass
          RegexpType::CharacterClass, std::string{bufv, ll},
          regexp_debug_info(this, "\\u", 2)};
    }
    case 'x': // hex: TODO
      lexer_error(*this, Errors::RegexpUnsupported, error_token(),
                  ErrorPosition::On, "Hex escapes not yet supported");
    default:
      lexer_error(*this, Errors::RegexpUnknown, error_token(),
                  ErrorPosition::On, "Unknown escape '%c'", c);
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Escape, c,
                    regexp_debug_info(this, "??", 2)};
    }
  }
  if (c == '^') {
    advance(1);
    return Regexp{std::string{source_p - 2, 2},
                  RegexpType::Assertion,
                  {RegexpAssertion::LineBeginning},
                  regexp_debug_info(this, "^", 1)};
  }
  if (c == '$') {
    advance(1);
    return Regexp{std::string{source_p - 2, 2},
                  RegexpType::Assertion,
                  {RegexpAssertion::LineEnd},
                  regexp_debug_info(this, "$", 1)};
  }
  // character class
  if (c == '[') {
    /*
      \[                               # character class = [[[
        \^?
        .+                             # simplified character classes
      \]                               # ]]]
    */
    int length = 0;
    int rlength = 0;
    bool inverted = false;
    advance(1);
    rlength++;
    if (*source_p == '^')
      inverted = true;
    else {
      advance(-1);
      rlength--;
    }
    if (inverted) {
      buffer[length++] = ']'; // TODO: get a decent way of specifying inversion
    }
    bool escaped = false;
    char lc = 0;
    c = 0;
    do {
      advance(1);
      rlength++;
      lc = c;
      c = *source_p;
      if (c == '\\') {
        if (escaped) {
          escaped = false;
          buffer[length++] = c;
          continue;
        } else {
          escaped = true;
          continue;
        }
      }
      if (!escaped && c == '-') {
        if (lc != 0) {
          int range_end_skip = 0;
          // specify range (a[buf]-.Z -> a-[buf+(a+1)...Z]Z.)
          advance(1);
          rlength++;
          c = *source_p;
          auto end_s = source_p;
          static char buf[6];
          if (c == '\\') {
            advance(1);
            rlength++;
            c = *source_p;
            if (c == 'u') {
              int len = 0;
              advance(1);
              rlength++;
              c = *source_p;
              c = tolower(c);
              while (isxdigit(c)) {
                if (len == 6)
                  break;
                buf[len++] = c;
                advance(1);
                rlength++;
                c = *source_p;
                c = tolower(c);
              }
              advance(-1);
              rlength--;
              if (len < 3) {
                lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                            ErrorPosition::On,
                            "Expected 4 to 6 hex digits after \\u, not %d",
                            len);
              }
              uint32_t cp = strtol(buf, NULL, 16);
              utf8_encode(buf, cp);
              end_s = buf;
              range_end_skip = 1;
            }
          }

          int range_end_len = Codepoints::getlength(end_s);
          range_end_skip = range_end_skip ?: range_end_len;
          int range_start_len = 1;
          unsigned char *pp = (unsigned char *)&buffer[length - 1];
          while ((*pp-- & 0b11000000) == 0b10000000)
            range_start_len++;
          pp++;
          uint32_t start, end;
          switch (range_start_len) {
          case 1: // 0xxxxxxx
            start = pp[0] & 0b01111111;
            break;
          case 2: // 110xxxxx 	10xxxxxx
            start = ((pp[0] & 0b00011111) << 6) | (pp[1] & 0b00111111);
            break;
          case 3: // 1110xxxx 	10xxxxxx 	10xxxxxx
            start = ((pp[0] & 0b00001111) << 12) | ((pp[1] & 0b00111111) << 6) |
                    (pp[2] & 0b00111111);
            break;
          case 4: // 11110xxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
            start = ((pp[0] & 0b00000111) << 18) |
                    ((pp[1] & 0b00111111) << 12) | ((pp[2] & 0b00111111) << 6) |
                    (pp[3] & 0b00111111);
            break;
          default:
            lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                        ErrorPosition::On,
                        "Charclass range start not expressible in unicode...?");
          }
          pp = (unsigned char *)end_s;
          switch (range_end_len) {
          case 1: // 0xxxxxxx
            end = pp[0] & 0b01111111;
            break;
          case 2: // 110xxxxx 	10xxxxxx
            end = ((pp[0] & 0b00011111) << 6) | (pp[1] & 0b00111111);
            break;
          case 3: // 1110xxxx 	10xxxxxx 	10xxxxxx
            end = ((pp[0] & 0b00001111) << 12) | ((pp[1] & 0b00111111) << 6) |
                  (pp[2] & 0b00111111);
            break;
          case 4: // 11110xxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
            end = ((pp[0] & 0b00000111) << 18) | ((pp[1] & 0b00111111) << 12) |
                  ((pp[2] & 0b00111111) << 6) | (pp[3] & 0b00111111);
            break;
          default:
            lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                        ErrorPosition::On,
                        "Charclass range start not expressible in unicode...?");
          }
          if (start >= end) {
            lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                        ErrorPosition::On,
                        "Charclass range out of order (start >= end : U+%08X "
                        ">= U+%08X)",
                        start, end);
          }
          for (uint32_t x = start + 1; x < end + 1; x++)
            length += utf8_encode(buffer + length, x);
          advance(range_end_skip - 1);
          rlength += range_end_skip - 1;
          continue;
        }
      }
      if (escaped && c == 'u') {
        escaped = false;
        static char buf[6];
        int len = 0;
        advance(1);
        c = *source_p;
        c = tolower(c);
        while (isxdigit(c)) {
          if (len == 6)
            break;
          buf[len++] = c;
          advance(1);
          c = *source_p;
          c = tolower(c);
        }
        advance(-1);
        if (len < 3) {
          lexer_error(*this, Errors::InvalidRegexpSyntax, error_token(),
                      ErrorPosition::On,
                      "Expected 4 to 6 hex digits after \\u, not %d", len);
        }
        uint32_t cp = strtol(buf, NULL, 16);
        length += utf8_encode(buffer + length, cp);
        continue;
      }
      if (!escaped && c == ']') {
        advance(1);
        break;
      }
      buffer[length++] = c;
    } while (1);
    return Regexp{"[" + std::string{source_p - rlength - 4, rlength + 3} + "]",
                  RegexpType::CharacterClass, std::string{buffer, length},
                  regexp_debug_info(this, "\\u", rlength + 3)};
  }
  // parenthesised expression
  if (c == '(') {
    nested_index++;
    advance(1);
    c = *source_p;
    if (c == '?') {
      // (?imIM:subexpr) -> set option
      advance(1);
      c = *source_p;
      if (strchr("imIM", c) != NULL) {
        const Token &mtoken = error_token();
        lexer_error(*this, Errors::RegexpUnsupported, mtoken, ErrorPosition::On,
                    "Subexpr options are not supported");
        return {};
      }
      if (c == '<' || c == '=') {
        const Token &mtoken = error_token();
        lexer_error(*this, Errors::RegexpUnsupported, mtoken, ErrorPosition::On,
                    "Lookarounds are not supported");
        return {};
      }
      if (c == ':') {
        // (?:...) shy group, set index -1, but increment index count anyway
        auto reg = regexp();
        if (!reg.has_value())
          return reg;
        auto &rv = reg.value();
        c = *source_p;
        if (c != ')') {
          const Token &mtoken = error_token();
          lexer_error(*this, Errors::InvalidRegexpSyntax, mtoken,
                      ErrorPosition::On, "Unbalanced parenthesis");
          return {};
        }
        advance(1);
        auto reg_ = Regexp{std::string{"(?:"} + rv.str + ")",
                           RegexpType::Nested, new Regexp{rv},
                           regexp_debug_info(this, "?:", rv.str.size() + 3)};
        reg_.index = -1;
        return reg_;
      }
    }
    auto my_index = nested_index;
    auto reg = regexp();
    if (!reg.has_value())
      return reg;
    auto &rv = reg.value();
    c = *source_p;
    if (c != ')') {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::InvalidRegexpSyntax, mtoken, ErrorPosition::On,
                  "Unbalanced parenthesis");
      return {};
    }
    advance(1); // )
    auto reg_ = Regexp{std::string{"("} + rv.str + ")", RegexpType::Nested,
                       new Regexp{rv},
                       regexp_debug_info(this, "()", rv.str.size() + 2)};
    reg_.index = my_index;
    return reg_;
  }
  if (c == ')') {
    return {};
  }
  const Token &mtoken = error_token();
  lexer_error(*this, Errors::RegexpUnknown, mtoken, ErrorPosition::On,
              "Unknown regular expression");
  return {};
}

// quantifier
std::optional<Regexp> NLexer::regexp_quantifier(Regexp &reg) {
  /*
  (?:                                  # optionally a quantifier
      (?:                              #
           [?+*]                       # lazy | plus | kleene
         | \{                          # {1} | {1,2}
             \d+
             (?:,\d*)?
           \}
      )
      [?+]?                            # lazy or plus attribute for lazy?
  )?                                   # optionally
  */
  char c = *source_p;
  const char *s = strchr("?+*{", c);
  if (s == NULL)
    return reg;
  switch (*s) {
  case '?':
    reg.lazy = true;
    advance(1);
    break;
  case '+':
    reg.plus = true;
    advance(1);
    break;
  case '*':
    reg.star = true;
    advance(1);
    break;
  case '{': {
    if (*(source_p + 1) == '{')
      return reg; // {{...}}
    // read numbers
    char numbuf[64];
    int length = 0;
    do {
      advance(1);
      c = *source_p;
      if (!isdigit(c))
        break;
      numbuf[length++] = c;
    } while (1);
    numbuf[length] = 0;
    int bound0 = atoi(numbuf);
    int bound1 = -1;
    bool has_second = false;
    if (c == ',') {
      has_second = true;
      int length = 0;
      do {
        advance(1);
        c = *source_p;
        if (!isdigit(c))
          break;
        numbuf[length++] = c;
      } while (1);
      numbuf[length] = 0;
      if (c != '}') {
        const Token &mtoken = error_token();
        lexer_error(*this, Errors::RegexpQuantifierRepeatInvalidSyntax, mtoken,
                    ErrorPosition::On,
                    "Expected either a number or '}' for the second bound of a "
                    "repeat quantifier");
        return {};
      }
      if (length != 0)
        bound1 = atoi(numbuf);
    } else if (c != '}') {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::RegexpQuantifierRepeatInvalidSyntax, mtoken,
                  ErrorPosition::On,
                  "Expected either ',' or '}' for a repeat quantifier");
      return {};
    }
    advance(1);
    reg.repeat = RepeatQuantifier{has_second, bound0, bound1};
    break;
  }
  }
  c = *source_p;
  if (c == '?') {
    reg.lazy = true;
    advance(1);
  } else if (c == '+') {
    reg.plus = true;
    advance(1);
  }
  return reg;
}

Regexp Regexp::concat(const Regexp &other) {
  if (type == RegexpType::Assertion && other.type == RegexpType::Assertion) {
    auto assertions = std::get<std::vector<RegexpAssertion>>(inner);
    std::vector<RegexpAssertion> asserts{assertions.cbegin(),
                                         assertions.cend()};
    for (auto as : std::get<std::vector<RegexpAssertion>>(other.inner))
      asserts.push_back(as);
    return Regexp{str + other.str,
                  RegexpType::Assertion,
                  asserts,
                  {debug_info.lineno, debug_info.offset,
                   debug_info.length + other.debug_info.length,
                   string_format("(cons %s %s)", debug_info.name.c_str(),
                                 other.debug_info.name.c_str())}};
  }
  if (type == RegexpType::CharacterClass) {
    if (other.type == RegexpType::Alternative) {
      bool yes = true;
      auto in = std::get<std::string>(inner);
      bool glinv = in[0] == '[';
      for (auto ch : other.children)
        if (ch->type != RegexpType::CharacterClass) {
          yes = false;
          break;
        } else if ((std::get<std::string>(ch->inner)[0] == '[' ^ glinv)) {
          yes = false;
          break;
        }
      if (yes) {
        std::string ss = in;
        for (auto ch : other.children)
          ss += std::get<std::string>(ch->inner).substr(0, (int)glinv);
        return Regexp{str + (std::string{"|"} + other.str),
                      RegexpType::CharacterClass,
                      ss,
                      {debug_info.lineno, debug_info.offset,
                       debug_info.length + other.debug_info.length + 1,
                       string_format("(or %s %s)", debug_info.name.c_str(),
                                     other.debug_info.name.c_str())}};
      }
    }
  }
  if (other.type == RegexpType::Alternative) {
    std::vector<Regexp *> vec;
    vec.push_back(new Regexp(*this));
    for (auto child : other.children)
      vec.push_back(child);
    return Regexp{str + (std::string{"|"} + other.str),
                  RegexpType::Alternative,
                  vec,
                  {debug_info.lineno, debug_info.offset,
                   debug_info.length + other.debug_info.length + 1,
                   string_format("(or %s %s)", debug_info.name.c_str(),
                                 other.debug_info.name.c_str())}};
  }
  if (type == RegexpType::Concat) {
    children.push_back(new Regexp{other});
    str += other.str;
    return *this;
  }
  std::vector<Regexp *> vec;
  vec.push_back(new Regexp{*this});
  vec.push_back(new Regexp{other});

  return Regexp(this->str + other.str, RegexpType::Concat, vec,
                {debug_info.lineno, debug_info.offset,
                 debug_info.length + other.debug_info.length,
                 string_format("(cons %s %s)", debug_info.name.c_str(),
                               other.debug_info.name.c_str())});
}

std::string Regexp::to_str() const {
  static char buf[1024];
  std::sprintf(buf, "Regexp { %d, ", type);
  std::string str = buf;
  if (type == RegexpType::Concat || type == RegexpType::Alternative)
    for (auto child : children)
      str +=
          child->to_str() + (type == RegexpType::Alternative ? " + " : " | ");
  else if (type == RegexpType::Nested)
    str += std::get<Regexp *>(inner)->to_str();
  else
    str += this->str;
  if (star)
    str += " *";
  if (plus)
    str += " +";
  if (lazy)
    str += " ?";
  str += " }";
  return str;
}

std::optional<std::string> NLexer::string(bool pescapes) {
  char c;
  int length = 0;
  if (!*source_p)
    return {};

  do {
    c = *(source_p++);
    offset++;
    if (c == 0)
      break;
    if (c == '\n') {
      lineno++;
      offset = 0;
      continue;
    }
    if (c != '"')
      continue;
    break;
  } while (1);
  bool escape = false;
  source_p--;
  offset--;
  do {
    advance(1);
    c = *source_p;
    if (c == 0)
      break;
    if (c == '"' && !escape)
      break;
    if (c == '\\') {
      if (escape)
        escape = false;
      else if (pescapes) {
        escape = true;
        continue;
      }
    } else if (escape) {
      c = escapes[c] ?: c;
      escape = false;
    }
    buffer[length++] = c;
  } while (1);
  advance(1);
  return std::string{buffer, length};
}

bool Regexp::sanity_check(const NLexer &lexer) {
  // simple sanity check
  if (star && plus) {
    // *+ not allowed
    std::printf("[I] Note: %s\n",
                "Two active quantifiers (star and plus) is not sane");
    return false;
  }
  if (repeat.has_value() && (star || plus || lazy)) {

    // {...}*+? not allowed
    std::printf("[I] Note: %s\n",
                "use of RepeatQuantifier with other quantifiers (star, plus, "
                "and lazy) is not sane");
    return false;
  }
  if (std::holds_alternative<std::vector<RegexpAssertion>>(inner) &&
      (star || plus || lazy || repeat.has_value())) {
    // Assertions cannot have quantifiers
    std::printf("[I] Note: %s\n",
                "use of assertions with other quantifiers (star, plus, "
                "lazy and repeat) is not sane");
    return false;
  }
  return true;
}

bool Regexp::operator==(const Regexp &other) const {
  if (type != other.type)
    return false;
  if (lazy ^ other.lazy || plus ^ other.plus || star ^ other.star)
    return false;
  if (repeat.has_value() ^ other.repeat.has_value())
    return false;
  {
    auto rp0 = repeat.value();
    auto rp1 = other.repeat.value();
    if (rp0.has_highbound ^ rp1.has_highbound)
      return false;
    if (rp0.lowbound != rp1.lowbound || rp0.highbound != rp1.highbound)
      return false;
  }

  switch (type) {
  case Dot:
    return true;
  case SubExprCall:
    return false; // TODO
  case CharacterClass:
    /* fallthrough */
  case Symbol:
    return std::get<std::string>(inner) == std::get<std::string>(other.inner);
  case Alternative: {
    if (children.size() != other.children.size())
      return false;
    for (int i = 0; i < children.size(); i++)
      if (*children[i] == *other.children[i])
        ;
      else
        return false;
    return true;
  }
  case Concat: {
    if (children.size() != other.children.size())
      return false;
    for (int i = 0; i < children.size(); i++)
      if (*children[i] == *other.children[i])
        ;
      else
        return false;
    return true;
  }
  case Nested:
    return *std::get<Regexp *>(inner) == *std::get<Regexp *>(other.inner);
  case Literal:
    /* fallthrough */
  case Escape:
    return std::get<char>(inner) == std::get<char>(other.inner);
  case Assertion:
    return std::get<std::vector<RegexpAssertion>>(inner) ==
           std::get<std::vector<RegexpAssertion>>(other.inner);
  }
}

void Regexp::resolve(
    const std::map<std::string, std::tuple<SymbolType, SymbolDebugInformation,
                                           std::variant<std::string, Regexp *>>>
        values) {

  switch (type) {
  case RegexpType::Literal:
  case RegexpType::Dot:
  case RegexpType::Escape:
  case RegexpType::CharacterClass:
  case RegexpType::Assertion:
    return;
  case RegexpType::Nested:
    return std::get<Regexp *>(inner)->resolve(values);
  case RegexpType::Alternative:
  case RegexpType::Concat:
    for (auto ch : children)
      ch->resolve(values);
    return;
  case RegexpType::Symbol: {
    const auto name = std::get<std::string>(inner);
    const auto &val = values.at(name);
    if (std::get<0>(val) == SymbolType::Define) {
      auto *reg = std::get<Regexp *>(std::get<2>(val));

      type = reg->type;
      children = reg->children;
      str = reg->str;
      inner = reg->inner;
      named_rule = reg->named_rule;
      debug_info = reg->debug_info;

      plus = plus || reg->plus;
      star = star || reg->star;
      lazy = lazy || reg->lazy;
      reg->is_leaf = false;
    } else /* Const */
    {
      // construct a literal regexp
      auto vv = std::get<2>(val);
      if (std::holds_alternative<std::string>(vv)) {
        std::string s = std::get<std::string>(vv);
        for (char c : s)
          children.push_back(new Regexp{
              std::string{c}, RegexpType::Literal, c, {0, 0, 0, "<Unknown>"}});
        str = s;
        type = RegexpType::Concat;
      } else {
        auto *reg = std::get<Regexp *>(vv);
        type = reg->type;
        children = reg->children;
        str = reg->str;
        inner = reg->inner;
        named_rule = "";
        debug_info = reg->debug_info;

        plus = plus || reg->plus;
        star = star || reg->star;
        lazy = lazy || reg->lazy;
        reg->is_leaf = false;
      }
    }
    was_reference = true;
    referenced_symbol = name;
    return;
  }
  default:
    /* code */
    break;
  }
}
std::string Regexp::mangle() const {
  auto s = str;
  return "/" + s + "/" + (star ? "*" : "") + (plus ? "+" : "") +
         (lazy ? "?" : "");
}

NFANode<std::string> *
Regexp::compile(std::multimap<const Regexp *, NFANode<std::string> *> &cache,
                NFANode<std::string> *parent, std::string path, bool &leading,
                bool nopath) const {
  assert(type != RegexpType::Symbol &&
         "Regexp must be completely resolved before compilation");
  if (was_reference)
    path = referenced_symbol.value();
  std::string namef = named_rule.has_value() ? named_rule.value() : path;
  std::string cpath =
      nopath ? (named_rule.has_value() ? named_rule.value() : path)
             : (named_rule.has_value() ? named_rule.value() : path) + "{::}" +
                   mangle();
  NFANode<std::string> *result;
  switch (type) {
  case RegexpType::Symbol: {
    std::printf("[ICE] unresolved symbol left in regular expression\n");
    abort();
  }
  case RegexpType::SubExprCall: {
    NFANode<std::string> *tl = new NFANode<std::string>{"R<>" + mangle()};
    parent->epsilon_transition_to(tl);
    tl->subexpr_call = subexprcall;
    tl->named_rule = namef;
    result = tl;
    result->debug_info = debug_info;
    break;
  }
  case RegexpType::Assertion: {
    // wat do?
    auto assert = std::get<std::vector<RegexpAssertion>>(inner);
    NFANode<std::string> *tl = new NFANode<std::string>{"A" + mangle()};
    parent->epsilon_transition_to(tl);
    tl->assertions = assert;
    tl->named_rule = namef;
    result = tl;
    result->debug_info = debug_info;
    break;
  }
  case RegexpType::Literal: {
    char t = std::get<char>(inner);
    NFANode<std::string> *tl = new NFANode<std::string>{"B" + mangle()};
    NFANode<std::string> *tl2 = new NFANode<std::string>{"E" + mangle()};
    tl->debug_info = debug_info;
    tl2->debug_info = debug_info;
    tl->transition_to(tl2, t);
    tl = transform_by_quantifiers(
        new PseudoNFANode<std::string>{"S" + mangle(), tl, tl2});
    parent->epsilon_transition_to(tl);
    tl->named_rule = namef;
    tl2->named_rule = namef;
    result = tl;
    break;
  }
  case RegexpType::Dot: {
    auto tv = new NFANode<std::string>{mangle()};
    tv->debug_info = debug_info;
    NFANode<std::string> *tl = transform_by_quantifiers(tv);
    tl->debug_info = debug_info;
    parent->anything_transition_to(tl);
    tl->named_rule = namef;
    cache.insert({this, tl});
    return tl;
  }
  case RegexpType::Escape:
    printf("Fuck no for now\n");
    return nullptr;
  case RegexpType::CharacterClass: {
    auto s = std::get<std::string>(inner);
    bool inv = false;
    if (s.size() > 1 && s[0] == ']') {
      s.erase(s.begin());
      inv = true;
    }
    NFANode<std::string> *tl =
        new NFANode<std::string>{cpath + "{::}" + "B" + mangle()};
    tl->debug_info = debug_info;
    NFANode<std::string> *tl2 =
        new NFANode<std::string>{cpath + "{::}" + "E" + mangle()};
    tl2->debug_info = debug_info;
    NFANode<std::string> *tl_1 =
        new NFANode<std::string>{cpath + "{::}" + "B_C1" + mangle()};
    tl_1->debug_info = debug_info;
    NFANode<std::string> *tl_2 =
        new NFANode<std::string>{cpath + "{::}" + "B_C2" + mangle()};
    tl_2->debug_info = debug_info;
    NFANode<std::string> *tl_3 =
        new NFANode<std::string>{cpath + "{::}" + "B_C3" + mangle()};
    tl_3->debug_info = debug_info;
    NFANode<std::string> *tl_end = tl2;
    tl->named_rule = namef;
    tl_1->named_rule = namef;
    tl_2->named_rule = namef;
    tl_3->named_rule = namef;

    if (!inv) {
      // we can transform this to many transitions instead
    } else {
      tl_end = new NFANode<std::string>{cpath + "{::}" + "Moot:E" + mangle()};
      tl_end->debug_info = debug_info;
      tl->default_transition_to(tl2);
    }

    for (auto cus : codepoints(s)) {
      if (cus.size() == 1)
        tl->transition_to(tl_end, cus[0]);
      else if (cus.size() == 2) {
        tl->transition_to(tl_1, cus[0]);
        tl_1->transition_to(tl_end, cus[1]);
      } else if (cus.size() == 3) {
        tl->transition_to(tl_1, cus[0]);
        tl_1->transition_to(tl_2, cus[1]);
        tl_2->transition_to(tl_end, cus[2]);
      } else if (cus.size() == 4) {
        tl->transition_to(tl_1, cus[0]);
        tl_1->transition_to(tl_2, cus[1]);
        tl_2->transition_to(tl_3, cus[2]);
        tl_3->transition_to(tl_end, cus[3]);
      }
    }

    tl = transform_by_quantifiers(
        new PseudoNFANode<std::string>{"S" + mangle(), tl, tl2});
    parent->epsilon_transition_to(tl);
    tl->named_rule = namef;
    tl2->named_rule = namef;
    result = tl;
    break;
  }
  case RegexpType::Nested: {
    NFANode<std::string> *tl = new NFANode<std::string>{"Bg" + mangle()};
    NFANode<std::string> *te = new NFANode<std::string>{"Eg" + mangle()};
    tl->debug_info = debug_info;
    te->debug_info = debug_info;

    auto ex = std::get<Regexp *>(inner);
    auto nfa = ex->compile(cache, tl, cpath + "{::}" + ex->mangle(), leading);
    nfa->epsilon_transition_to(te);

    tl->subexpr_idx = index;
    tl->named_rule = namef;
    te->named_rule = namef;
    tl = new PseudoNFANode<std::string>{"S" + mangle(), tl, te};

    tl = transform_by_quantifiers(tl);
    parent->epsilon_transition_to(tl);
    tl->named_rule = namef;
    result = tl;
    break;
  }
  case RegexpType::Alternative: {
    // generate all nodes and connect them all to a root node
    NFANode<std::string> *tl = new NFANode<std::string>{"Ba" + mangle()};
    NFANode<std::string> *te = new NFANode<std::string>{"Ea" + mangle()};
    tl->debug_info = debug_info;
    te->debug_info = debug_info;
    for (auto *alt : children) {
      bool leading_ = true;
      auto p = alt->compile(cache, tl, cpath, leading_);
      // std::printf("yay %s = ", alt->mangle().c_str());
      // p->print();
      // std::printf("\n");
      p->epsilon_transition_to(te);
      // cache[alt->mangle()] = p;
    }
    te->named_rule = namef;
    tl = new PseudoNFANode<std::string>{"S" + mangle(), tl, te};
    tl = transform_by_quantifiers(tl);
    parent->epsilon_transition_to(tl);
    // std::printf("yay %s = ", mangle().c_str());
    // tl->print();
    // std::printf("\n");
    // cache[cpath] = tl->deep_copy();
    tl->named_rule = namef;
    // if (was_reference)
    // cache[referenced_symbol.value()] = tl->deep_copy();
    result = tl;
    break;
  }
  case RegexpType::Concat: {
    NFANode<std::string> *root = new NFANode<std::string>{"B" + mangle()};
    NFANode<std::string> *endp = new NFANode<std::string>{"E" + mangle()};
    root->debug_info = debug_info;
    endp->debug_info = debug_info;
    root->named_rule = namef;
    // generate all nodes and add transitions between them
    bool leading_ = leading;
    auto p = children[0]->compile(
        cache, root, cpath + "{::}" + children[0]->mangle(), leading_);
    NFANode<std::string> *prev_s = p;
    bool b = true;
    for (auto c : children) {
      cpath = cpath + "{::}" + c->mangle();
      if (b) {
        b = false;
        continue;
      } else {
        prev_s = c->compile(cache, prev_s, cpath, leading_);
      }
    }
    prev_s->epsilon_transition_to(endp);
    root = transform_by_quantifiers(
        new PseudoNFANode<std::string>{"S" + mangle(), root, endp});
    parent->epsilon_transition_to(root);
    root->named_rule = namef;
    endp->named_rule = namef;
    result = root;
    break;
  }
  }
  if (was_reference)
    result->reference_node = true;
  cache.insert({this, result});
  return result;
}

NFANode<std::string> *
Regexp::transform_by_quantifiers(NFANode<std::string> *node) const {
  if (star) {
    // A -e-> A
    auto tl = new NFANode<std::string>{"B*" + mangle()};
    auto tp = new NFANode<std::string>{"E*" + mangle()};
    tl->epsilon_transition_to(node);
    node->epsilon_transition_to(tp);
    tp->epsilon_transition_to(tl);
    tl->epsilon_transition_to(tp);
    // tp->epsilon_transition_to(tl);
    node = new PseudoNFANode<std::string>{"S*" + mangle(), tl, tp};
  } else if (plus) {
    // -e-> A -e-> A
    auto tl = new NFANode<std::string>{"B+" + mangle()};
    tl->epsilon_transition_to(node);
    node->epsilon_transition_to(node);
    node = new PseudoNFANode<std::string>{"S+" + mangle(), tl, node};
  } else if (repeat.has_value()) {
    auto rp = repeat.value();
    auto tl = new NFANode<std::string>{"Br" + mangle()};
    auto tp = new NFANode<std::string>{"Er" + mangle()};
    tl->epsilon_transition_to(node);
    // repeat the compilation many times (sorry...)
    // TODO: fix deep copy

    // repeat lowbound times
    auto rgc = const_cast<Regexp *>(this); // putting it back, probably ok...?
    auto lz = rgc->lazy;
    auto st = rgc->star;
    rgc->repeat.reset(); // don't repeat the copy itself
    rgc->lazy = false;
    std::multimap<const Regexp *, NFANode<std::string> *> node_cache;
    auto leading_ = false;
    for (auto i = 0; i < rp.lowbound - 2; i++)
      node = rgc->compile(node_cache, node,
                          string_format("%s{::}%s{::}Rpt%d", mangle().c_str(),
                                        rgc->mangle().c_str(), i),
                          leading_);

    if (rp.has_highbound) {
      if (rp.highbound == -1) {
        auto tl0 = new NFANode<std::string>{"Br*" + mangle()};
        auto tp0 = new NFANode<std::string>{"Er*" + mangle()};
        auto tnode =
            rgc->compile(node_cache, tl0,
                         string_format("%s{::}%s{::}Rpt%d", mangle().c_str(),
                                       rgc->mangle().c_str(), rp.lowbound - 2),
                         leading_);

        tl0->epsilon_transition_to(tnode);
        tnode->epsilon_transition_to(tp0);
        tp0->epsilon_transition_to(tl0);
        tl0->epsilon_transition_to(tp0);
        node->epsilon_transition_to(tl0);
        node = tp0;
        rgc->lazy = lz;
        rgc->repeat = rp;
      } else {
        node =
            rgc->compile(node_cache, node,
                         string_format("%s{::}%s{::}Rpt%d", mangle().c_str(),
                                       rgc->mangle().c_str(), rp.lowbound - 2),
                         leading_);
        rgc->lazy = true;
        for (auto i = rp.lowbound; i < rp.highbound; i++)
          node =
              rgc->compile(node_cache, node,
                           string_format("%s{::}%s{::}Rpt%d", mangle().c_str(),
                                         rgc->mangle().c_str(), i),
                           leading_);
        rgc->lazy = lz;
        rgc->repeat = rp;
      }
    } else {
      node = rgc->compile(node_cache, node,
                          string_format("%s{::}%s{::}Rpt%d", mangle().c_str(),
                                        rgc->mangle().c_str(), rp.lowbound - 2),
                          leading_);
      rgc->lazy = lz;
      rgc->repeat = rp;
    }
    node->epsilon_transition_to(tp);
    node = new PseudoNFANode<std::string>{"Sr" + mangle(), tl, tp};
  }

  if (lazy) {
    // t -e-> A -e-> e, t -e-> e
    auto tl = new NFANode<std::string>{"B?" + mangle()};
    auto tp = new NFANode<std::string>{"E?" + mangle()};
    tl->epsilon_transition_to(node);
    tl->epsilon_transition_to(tp);
    node->epsilon_transition_to(tp);
    node = new PseudoNFANode<std::string>{"S?" + mangle(), tl, tp};
  }
  return node;
}

template <typename T> void NFANode<T>::transition_to(NFANode<T> *node, char c) {
  node = deep_input_end(node);
  auto p = deep_output_end(this);
  auto t = new Transition<
      NFANode<T>, std::variant<char, EpsilonTransitionT, AnythingTransitionT>>(
      node, c);
  p->add_transition(t);
}
template <typename T>
void NFANode<T>::transition_to(NFANode<T> *node, AnythingTransitionT c) {
  node = deep_input_end(node);
  auto p = deep_output_end(this);
  auto t = new Transition<
      NFANode<T>, std::variant<char, EpsilonTransitionT, AnythingTransitionT>>(
      node, c);
  p->add_transition(t);
}

template <typename T> void NFANode<T>::epsilon_transition_to(NFANode<T> *node) {
  node = deep_input_end(node);
  auto p = deep_output_end(this);
  auto t = new Transition<
      NFANode<T>, std::variant<char, EpsilonTransitionT, AnythingTransitionT>>(
      node, EpsilonTransition);
  p->add_transition(t);
}

template <typename T>
void NFANode<T>::anything_transition_to(NFANode<T> *node) {
  node = deep_input_end(node);
  auto p = deep_output_end(this);
  auto t = new Transition<
      NFANode<T>, std::variant<char, EpsilonTransitionT, AnythingTransitionT>>(
      node, AnythingTransition);
  p->add_transition(t);
}
template <typename StateInfoT>
NFANode<StateInfoT> *NFANode<StateInfoT>::copy_if(bool pred) {
  if (!pred) {
    return this;
  }
  auto *p = dynamic_cast<PseudoNFANode<StateInfoT> *>(this);
  if (p == nullptr)
    return new NFANode<StateInfoT>{*this};
  else
    return new PseudoNFANode<StateInfoT>{*p};
}

template <typename StateInfoT>
NFANode<StateInfoT> *NFANode<StateInfoT>::deep_copy() {
  auto *node = new NFANode<StateInfoT>();
  node->state_info = state_info;
  node->outgoing_transitions = outgoing_transitions;
  node->start = start;
  node->final = final;
  auto ps0 = decltype(node->outgoing_transitions){},
       ps1 = decltype(node->outgoing_transitions){};
  for (auto o = node->outgoing_transitions.begin();
       o != node->outgoing_transitions.end(); ++o) {
    auto x = new typename std::remove_pointer<
        typename std::remove_reference<decltype(*o)>::type>::type{
        (*o)->target->deep_copy(), (*o)->input};
    ps0.insert(*o);
    ps1.insert(x);
  }
  for (auto a : ps0)

    node->outgoing_transitions.erase(a);

  for (auto a : ps1)

    node->outgoing_transitions.insert(a);
  return node;
}
template <typename StateInfoT>
NFANode<StateInfoT> *PseudoNFANode<StateInfoT>::deep_copy() {
  auto *node = new PseudoNFANode<StateInfoT>{
      (this->state_info.has_value() ? this->state_info.value() : StateInfoT{}),
      this->input_end->deep_copy(), this->output_end->deep_copy()};
  node->outgoing_transitions = this->outgoing_transitions;
  node->start = this->start;
  node->final = this->final;
  auto ps0 = decltype(node->outgoing_transitions){},
       ps1 = decltype(node->outgoing_transitions){};
  for (auto o = node->outgoing_transitions.begin();
       o != node->outgoing_transitions.end(); ++o) {
    auto x = new typename std::remove_pointer<
        typename std::remove_reference<decltype(*o)>::type>::type{
        (*o)->target->deep_copy(), (*o)->input};
    ps0.insert(*o);
    ps1.insert(x);
  }
  for (auto a : ps0)
    node->outgoing_transitions.erase(a);

  for (auto a : ps1)
    node->outgoing_transitions.insert(a);

  return node;
}
template <typename StateInfoT>
std::set<Transition<NFANode<StateInfoT>, std::variant<char, EpsilonTransitionT,
                                                      AnythingTransitionT>> *,
         TransitionPointerComparer<StateInfoT>>
NFANode<StateInfoT>::get_outgoing_transitions(bool inner) {
  return outgoing_transitions;
}

template <typename StateInfoT>
std::set<Transition<NFANode<StateInfoT>, std::variant<char, EpsilonTransitionT,
                                                      AnythingTransitionT>> *,
         TransitionPointerComparer<StateInfoT>>
PseudoNFANode<StateInfoT>::get_outgoing_transitions(bool inner) {
  if (inner)
    return deep_input_end(this)->get_outgoing_transitions();
  else
    return deep_output_end(this)->get_outgoing_transitions();
}

// #define TEST
#ifdef TEST
int main() {
  NLexer lexer{R"(
  option lemmatise on
  stopword "test" "fest" -"stopwords.list"
  boo     :- "testt"
  test    :- "43"
  t       <= test
  boop    :: ({{boo}}|{{test}}\d)+
  )"};
  do {
    Token tok = lexer.next();
    if (tok.type == TOK_EOF)
      break;
    tok.print();
  } while (1);
}
#endif
