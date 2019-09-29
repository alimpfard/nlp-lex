#include "lexer.hpp"
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
};

void Token::print() {
  if (type == TokenType::TOK_BOOL)
    std::printf("Token { %s, line %d, offset %d, length %d, value = <%s> }\n",
                reverse_token_type[type], lineno, offset, length,
                std::get<bool>(value) ? "true" : "false");
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
      const Token &mtoken = {TOK_ERROR, lineno, offset, 0, empty_string};
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
    const Token &mtoken = {TOK_ERROR, lineno, offset, 0, empty_string};
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
      const Token &mtoken = {TOK_ERROR, lineno, offset, 0, empty_string};
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
      const Token &mtoken = {TOK_ERROR, lineno, offset, 0, empty_string};
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
    auto opt = regexp();
    if (!opt.has_value()) {
        const Token &mtoken = {TOK_ERROR, lineno, offset, 0, empty_string};
        lexer_error(*this, Errors::InvalidRegexp, mtoken, ErrorPosition::On, "Expected a valid regex (see above for possible diagnosis)");
        return mtoken;
    }
    Regexp regex = opt.value();
    return Token{TOK_REGEX, lineno, offset-regex.str.size(), regex.str.size(), regex};
  }
  default:
    /* code */
    break;
  }
}

std::optional<Regexp> NLexer::regexp() {
    // TODO
    return {};
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

#define TEST
#ifdef TEST
int main() {
  NLexer lexer{R"(
  option lemmatise on
  stopword "test" "fest" -"stopwords.list"
  boo     :- "testt"
  test    :- "43"
  t       <= test
  boop    :: {{boo}}{{test}}
  )"};
  do {
    Token tok = lexer.next();
    if (tok.type == TOK_EOF)
      break;
    tok.print();
  } while (1);
}
#endif
