#include "lexer.hpp"
#include <cassert>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstring>

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
      c = *(source_p++);
      offset++;
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
    if (prev_token.length == 1 && c == '<') {
      // norm
      if (strncmp("=", source_p, 1) != 0) {
        lexer_error(*this, Errors::Unexpected, mtoken, ErrorPosition::On,
                    "Expected `<='");
        return mtoken;
      }
      advance(1);
      state = LexerState::Normal;
      return Token{TOK_OPNORMAL, lineno, offset, 2, empty_string};
    }
    if (c != ':') {
      lexer_error(*this, Errors::Unexpected, mtoken, ErrorPosition::On,
                  "Expected either `::' or `:-'");
      return mtoken;
    }

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
  case LexerState::Normal: {
    length = 0;
    buffer[length++] = c;
    while (1) {
      c = *source_p;
      if (c == '\0' || c == '\n')
        break;
      buffer[length++] = c;
      advance(1);
    }
    buffer[length] = 0;
    state = LexerState::Toplevel;
    return Token{TOK_LITSTRING, lineno, offset - length, length,
                 std::string{buffer, length}};
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
    return Regexp{std::string{source_p - length - 5, length + 4},
                  RegexpType::Symbol, std::string{buffer, length}};
  }
  if (c == '|') {
    // is pipe, generate alternative
    advance(1);
    auto alt = regexp();
    if (!alt.has_value())
      return alt;
    std::vector<Regexp *> vec;
    vec.push_back(new Regexp{alt.value()});
    return Regexp{alt.value().str, RegexpType::Alternative, vec};
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
  if (strchr("?+*{}()[]\\|.", c) == NULL) {
    advance(1);
    return Regexp{std::string{source_p - 1, 1}, RegexpType::Literal, c};
  }
  if (c == '.') {
    // match anything
    advance(1);
    return Regexp{std::string{source_p - 1, 1}, RegexpType::Dot, c};
  }
  // escaped character
  if (c == '\\') {
    advance(1);
    c = *source_p;
    advance(1);
    switch (c) {
    case 'x': // hex and unicode: TODO
    case 'u':
      lexer_error(*this, Errors::RegexpUnsupported, error_token(),
                  ErrorPosition::On, "Unicode/Hex escapes not yet supported");
    default:
      return Regexp{std::string{source_p - 2, 2}, RegexpType::Escape, c};
    }
  }
  // character class
  if (c == '[') {
    /*
      \[                               # character class = [[[
        .+                             # simplified character classes
      \]                               # ]]]
    */
    int length = 0;
    do {
      advance(1);
      c = *source_p;
      if (c == ']')
        break;
      buffer[length++] = c;
    } while (1);
    return Regexp{std::string{source_p - length - 1, length + 1},
                  RegexpType::CharacterClass, std::string{buffer, length - 1}};
  }
  // parenthesised expression
  if (c == '(') {
    advance(1);
    c = *source_p;
    if (c == '?') {
      const Token &mtoken = error_token();
      lexer_error(*this, Errors::RegexpUnsupported, mtoken, ErrorPosition::On,
                  "Lookarounds are not supported");
      return {};
    }
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
    return Regexp{std::string{"("} + rv.str + ")", RegexpType::Nested,
                  new Regexp{rv}};
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
  if (other.type == RegexpType::Alternative) {
    std::vector<Regexp *> vec;
    vec.push_back(new Regexp(*this));
    for (auto child : other.children)
      vec.push_back(child);
    return Regexp{str + (std::string{"|"} + other.str), RegexpType::Alternative,
                  vec};
  }
  if (type == RegexpType::Concat) {
    children.push_back(new Regexp{other});
    str += other.str;
    return *this;
  }
  std::vector<Regexp *> vec;
  vec.push_back(new Regexp{*this});
  vec.push_back(new Regexp{other});

  return Regexp(this->str + other.str, RegexpType::Concat, vec);
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

std::optional<std::string> NLexer::string() {
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
      else {
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
  return true;
}

bool Regexp::operator==(const Regexp &other) const {
  if (type != other.type)
    return false;
  if (lazy ^ other.lazy || plus ^ other.plus || star ^ other.star)
    return false;
  // TODO: check RepeatQuantifier

  switch (type) {
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

      plus = plus || reg->plus;
      star = star || reg->star;
      lazy = lazy || reg->lazy;
      reg->is_leaf = false;
    } else /* Const */ {
      // construct a literal regexp
      std::string s = std::get<std::string>(std::get<2>(val));
      for (char c : s)
        children.push_back(new Regexp{std::string{c}, RegexpType::Literal, c});
      str = s;
      type = RegexpType::Concat;
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
Regexp::compile(std::map<std::string, NFANode<std::string> *> &cache,
                NFANode<std::string> *parent, std::string path,
                bool nopath) const {
  assert(type != RegexpType::Symbol &&
         "Regexp must be completely resolved before compilation");
  std::string cpath = nopath ? path : path + "{::}" + mangle();
  // std::printf("cache lookup for '%s'\n", cpath.c_str());
  if (cache.count(cpath) > 0) {
    // std::printf("cache hit for '%s'\n", cpath.c_str());
    return cache[cpath];
  }
  switch (type) {
  case RegexpType::Literal: {
    char t = std::get<char>(inner);
    NFANode<std::string> *tl =
        transform_by_quantifiers(new NFANode<std::string>{mangle()});
    parent->transition_to(tl, t);
    cache[cpath] = tl;
    return tl;
  }
  case RegexpType::Dot: {
    NFANode<std::string> *tl =
        transform_by_quantifiers(new NFANode<std::string>{mangle()});
    parent->anything_transition_to(tl);
    cache[cpath] = tl;
    return tl;
  }
  case RegexpType::Escape:
  case RegexpType::CharacterClass:
    printf("Fuck no for now\n");
    return nullptr;
  case RegexpType::Nested: {
    NFANode<std::string> *tl =
        transform_by_quantifiers(new NFANode<std::string>{mangle()});
    parent->epsilon_transition_to(tl);
    auto *exp = std::get<Regexp *>(inner)->compile(cache, tl, cpath);
    cache[cpath] = exp;
    return exp;
  }
  case RegexpType::Alternative: {
    // generate all nodes and connect them all to a root node
    NFANode<std::string> *tl = new NFANode<std::string>{mangle()};
    for (auto *alt : children) {
      auto p = alt->compile(cache, tl, cpath, true);
      // cache[alt->mangle()] = p;
    }
    tl = transform_by_quantifiers(tl);
    parent->epsilon_transition_to(tl);
    cache[cpath] = tl;
    return tl;
  }
  case RegexpType::Concat: {
    NFANode<std::string> *root = new NFANode<std::string>{mangle()};
    // generate all nodes and add transitions between them
    auto p = children[0]->compile(cache, root, cpath);
    // cache[children[0]->mangle()] = p;
    NFANode<std::string> *tl = p, *prev_s = p;
    bool b = true;
    for (auto c : children) {
      cpath = cpath + "{::}" + c->mangle();
      if (b) {
        b = false;
        continue;
      } else {
        prev_s = c->compile(cache, prev_s, cpath);
        // cache[c->mangle()] = p;
      }
    }
    root = transform_by_quantifiers(
        new PseudoNFANode<std::string>{"S" + mangle(), root, prev_s});
    parent->epsilon_transition_to(root);
    cache[cpath] = root;
    return root;
  }
  }
}

NFANode<std::string> *
Regexp::transform_by_quantifiers(NFANode<std::string> *node) const {
  // TODO
  return node;
}

template <typename T> void NFANode<T>::transition_to(NFANode<T> *node, char c) {
  get_output_end()->outgoing_transitions.push_back(
      Transition<NFANode<T>,
                 std::variant<char, EpsilonTransitionT, AnythingTransitionT>>(
          node->get_input_end(), c));
}

template <typename T> void NFANode<T>::epsilon_transition_to(NFANode<T> *node) {
  get_output_end()->outgoing_transitions.push_back(
      Transition<NFANode<T>,
                 std::variant<char, EpsilonTransitionT, AnythingTransitionT>>(
          node->get_input_end(), EpsilonTransition));
}

template <typename T>
void NFANode<T>::anything_transition_to(NFANode<T> *node) {
  get_output_end()->outgoing_transitions.push_back(
      Transition<NFANode<T>,
                 std::variant<char, EpsilonTransitionT, AnythingTransitionT>>(
          node->get_input_end(), AnythingTransition));
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
