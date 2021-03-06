#pragma once

#include <cstring>
#include <optional>
#include <stdarg.h>
#include <string>
#include <variant>
#include <vector>

#include "codepoints.hpp"
#include "regexp.hpp"

enum TokenType {
  TOK_OPTION,
  TOK_STOPWORD,
  TOK_IGNORE,
  TOK_OPCONST,
  TOK_OPDEFINE,
  TOK_OPLIT,
  TOK_KDEFINE,
  TOK_KDEFINE_CODE,
  TOK_OPNORMAL,
  TOK_LITSTRING,
  TOK_FILESTRING,
  TOK_BOOL,
  TOK_NAME,
  TOK_REGEX,
  TOK_EOF,
  TOK_CBRAC, // terminate ignore
  TOK_TAG,
  TOK_TAGPOS,
  TOK_TAGPOSWITH,
  TOK_TAGPOSFROM,
  TOK_TAGPOSEVERY,
  TOK_TAGPOSDELIM,
  TOK_ERROR
};

static char *reverse_token_type[TOK_ERROR + 1] = {
    [TOK_OPTION] = "Option",
    [TOK_STOPWORD] = "Stopword",
    [TOK_IGNORE] = "Ignore",
    [TOK_OPCONST] = "Const",
    [TOK_OPDEFINE] = "Define",
    [TOK_OPLIT] = "Literal",
    [TOK_KDEFINE] = "KaleidDefine",
    [TOK_KDEFINE_CODE] = "KaleidDefineCode",
    [TOK_OPNORMAL] = "Normalise",
    [TOK_LITSTRING] = "LiteralString",
    [TOK_FILESTRING] = "FileString",
    [TOK_BOOL] = "Boolean",
    [TOK_NAME] = "Name",
    [TOK_REGEX] = "Regexp",
    [TOK_EOF] = "EOF",
    [TOK_CBRAC] = "CloseBracket",
    [TOK_TAG] = "Tag",
    [TOK_TAGPOS] = "Pos",
    [TOK_TAGPOSWITH] = "TagPosWith",
    [TOK_TAGPOSFROM] = "TagPosFrom",
    [TOK_TAGPOSEVERY] = "TagPosEvery",
    [TOK_TAGPOSDELIM] = "TagPosDelimiter",
    [TOK_ERROR] = "Error"};

struct Token {
  TokenType type;
  int lineno;
  int offset;
  int length;
  std::variant<std::string, bool, Regexp, int> value;

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

enum class LexerState : int {
  Toplevel = 0, // valid tokens:
                /* Option   -> Option
                 * Stopword -> Stopword
                 * Name     -> Name
                 * Comment  -> /
                 * define   -> KaleidDefine
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
  Ignore,       // [ -> IgnoreBrac

  IgnoreBrac, // valid tokens
              // Name -> IgnoreBrac
              // ] -> Toplevel

  String,    // -> String
  Const,     // String -> Toplevel
  NormalSrc, // -> NormalTo
  NormalTo,  // -> NormalTgt
  NormalTgt, // -> Toplevel
  Define,    // -> Toplevel (parses a regex)
  Literal,   // -> Toplevel (parses a sequence of strings)

  KaliedDefine, // until newline -> KaliedDefine
                // newline -> Toplevel

  Tag,    // 'pos' -> TagPOS
  TagPOS, // 'from' -> TagPOSFrom
          // 'every' -> TagPOSEvery
          // 'with' -> TagPOSWith
          // 'delimiter' -> TagPOSWith
          // * -> Toplevel

  TagPOSFrom, // -> Toplevel (reads string)
              // 'every' -> TagPOSEvery
              // 'with' -> TagPOSWith
              // 'delimiter' -> TagPOSWith

  TagPOSEvery, // number -> TagPOSEveryToken

  TagPOSEveryToken,
  // 'with' -> TagPOSWith
  // 'delimiter' -> TagPOSDelimiter
  // 'from' -> TagPOSFrom

  TagPOSWith,
  // 'delimiter' -> TagPOSDelimiter

  TagPOSDelimiter,
  TagPOSDelimiterName,
  TagPOSDelimiterValue,
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
  InvalidCodepointSpecification,
  UnknownUnicodeClassName,
  LAST
};

class NLexer {
  Token _next();
  Token prev_token;

public:
  std::string lxst2str[((int)LexerState::TagPOSDelimiterValue) + 1] = {
      [(int)LexerState::Toplevel] = "Toplevel",
      [(int)LexerState::Name] = "Name",
      [(int)LexerState::Option] = "Option",
      [(int)LexerState::OptionBool] = "OptionBool",
      [(int)LexerState::Stopword] = "Stopword",
      [(int)LexerState::Ignore] = "Ignore",
      [(int)LexerState::IgnoreBrac] = "IgnoreBrac",
      [(int)LexerState::String] = "String",
      [(int)LexerState::Const] = "Const",
      [(int)LexerState::NormalSrc] = "NormalSrc",
      [(int)LexerState::NormalTo] = "NormalTo",
      [(int)LexerState::NormalTgt] = "NormalTgt",
      [(int)LexerState::Define] = "Define",
      [(int)LexerState::Literal] = "Literal",
      [(int)LexerState::KaliedDefine] = "KaleidDefine",
      [(int)LexerState::Tag] = "Tag",
      [(int)LexerState::TagPOS] = "TagPOS",
      [(int)LexerState::TagPOSFrom] = "TagPOSFrom",
      [(int)LexerState::TagPOSEvery] = "TagPOSEvery",
      [(int)LexerState::TagPOSEveryToken] = "TagPOSEveryToken",
      [(int)LexerState::TagPOSWith] = "TagPOSWith",
      [(int)LexerState::TagPOSDelimiter] = "TagPOSDelimiter",
      [(int)LexerState::TagPOSDelimiterName] = "TagPOSDelimiterName",
      [(int)LexerState::TagPOSDelimiterValue] = "TagPOSDelimiterValue",
  };
  std::string source;

  char *source_p;
  char *old_source_p;
  char *old_old_source_p;

  int lineno;
  int offset;
  char buffer[1024000];
  int nested_index = 0;
  int inside_index = 0;
  /// If there's anything on this, reset index to it
  /// when matching alternatives
  std::stack<int> branch_reset_indices{};

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
  std::optional<std::string> string(bool pescapes = true);
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

static std::string string_format(const std::string fmt_str, ...) {
  int final_n,
      n = ((int)fmt_str.size()) *
          2; /* Reserve two times as much as the length of the fmt_str */
  std::unique_ptr<char[]> formatted;
  va_list ap;
  while (1) {
    formatted.reset(new char[n]); /* Wrap the plain char array into the
                                         unique_ptr */
    strcpy(&formatted[0], fmt_str.c_str());
    va_start(ap, fmt_str);
    final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
    va_end(ap);
    if (final_n < 0 || final_n >= n)
      n += abs(final_n - n + 1);
    else
      break;
  }
  return std::string(formatted.get());
}
