#pragma once

#include <cstring>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "regexp.hpp"

enum TokenType {
  TOK_OPTION,
  TOK_STOPWORD,
  TOK_OPCONST,
  TOK_OPDEFINE,
  TOK_OPNORMAL,
  TOK_LITSTRING,
  TOK_FILESTRING,
  TOK_BOOL,
  TOK_NAME,
  TOK_REGEX,
  TOK_EOF,
  TOK_ERROR
};

char *reverse_token_type[TOK_ERROR + 1] = {[TOK_OPTION] = "Option",
                                           [TOK_STOPWORD] = "Stopword",
                                           [TOK_OPCONST] = "Const",
                                           [TOK_OPDEFINE] = "Define",
                                           [TOK_OPNORMAL] = "Normalise",
                                           [TOK_LITSTRING] = "LiteralString",
                                           [TOK_FILESTRING] = "FileString",
                                           [TOK_BOOL] = "Boolean",
                                           [TOK_NAME] = "Name",
                                           [TOK_REGEX] = "Regexp",
                                           [TOK_EOF] = "EOF",
                                           [TOK_ERROR] = "Error"};

struct Token {
  TokenType type;
  int lineno;
  int offset;
  int length;
  std::variant<std::string, bool, Regexp> value;

public:
  void print();
};

static Token EOFToken = {
    TokenType::TOK_EOF,
    // value
    0,
    0,
    0,
};

enum LexerState {
  Toplevel = 0, // valid tokens:
                /* Option   -> Option
                 * Stopword -> Stopword
                 * Name     -> Name
                 * Comment  -> /
                 */
  Name,         // valid tokens:
                /* ::       -> Define
                 * :-       -> Const
                 * <=       -> Normal
                 */
  Option,       // valid tokens:
                /* Name     -> OptionBool
                 */
  OptionBool,   // -> Toplevel
  Stopword,     // valid tokens
                /* String -> Stopword
                 * newline -> Toplevel
                 */
  String,       // -> String
  Const,        // String -> Toplevel
  Normal,       // -> Toplevel
  // TODO vvv
  Define, // -> Toplevel (parses a regex)
};

enum ErrorPosition {
  Before,
  After,
  On,
};

static const std::string empty_string = "";

enum Errors {
  Unexpected,
  ExpectedValue,
  InvalidRegexp,
  RegexpExpectedSymbolExpression,
  RegexpInvalidSymbolExpression,
  RegexpUnsupported,
  RegexpUnknown,
  RegexpQuantifierRepeatInvalidSyntax,
  InvalidRegexpSyntax,
  LAST
};

class NLexer {
  Token _next();
  Token prev_token;

public:
  std::string source;

  char *source_p;
  char *old_source_p;
  char *old_old_source_p;

  int lineno;
  int offset;
  char buffer[1024];

  std::vector<Token> injected;

  LexerState state;
  static char const *errors[Errors::LAST];

  NLexer(std::string src) : source(src) {
    source_p = strdup(src.c_str());
    old_source_p = source_p;
    old_old_source_p = source_p;
    lineno = 0;
    offset = 0;
    state = LexerState::Toplevel;
    buffer[0] = 0;
  }

  const Token next();
  std::optional<std::string> string();
  std::optional<Regexp> _regexp();

  std::optional<Regexp> regexp();
  std::optional<Regexp> regexp_tl();
  std::optional<Regexp> regexp_main();
  std::optional<Regexp> regexp_expression();
  std::optional<Regexp> regexp_quantifier(Regexp &reg);
  std::optional<Regexp> regexp_extra_tl();

  void revert();
  void inject(Token);

  void comment(char c);
  void advance(int i);

  Token error_token();
};

void lexer_error(const NLexer &lexer, int errn, const Token &tok,
                 ErrorPosition pos, char const *fmt...);
