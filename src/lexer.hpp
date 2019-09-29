#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <variant>
#include <optional>

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

struct Token {
    TokenType type;
    int lineno;
    int offset;
    int length;
    std::variant<std::string, bool> value;
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
    // TODO vvv
    Define,       // -> Toplevel (parses a regex)
    Const,
    Normal,
};

enum ErrorPosition {
    Before,
    After,
    On,
};

static const std::string empty_string = "";

enum Errors {
    Unexpected,
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
    static char const* errors[Errors::LAST];

    NLexer(std::string src) : source(src) {
        source_p = strdup (src.c_str());
        old_source_p = source_p;
        old_old_source_p = source_p;
        lineno = 0;
        offset = 0;
        state = LexerState::Toplevel;
        buffer[0] = 0;
    }

    const Token next();
    std::optional<std::string> string();

    void revert();
    void inject(Token);

    void comment(char c);
    void advance(int i);
};

void lexer_error(const NLexer& lexer, int errn, const Token &tok, ErrorPosition pos, char const* fmt...);
