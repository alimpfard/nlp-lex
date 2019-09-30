#include "parser.hpp"
#include "lexer.hpp"

NFANode NParser::compile(std::string code) {
  lexer = std::make_unique<NLexer>(code);
  return compile();
}

NFANode NParser::compile() {
  statestack = {};
  statestack.push(ParserState::Toplevel);
  bool failing = false;
  std::variant<std::string> persist;
  do {
    Token token = lexer->next();
    if (token.type == TOK_EOF)
      break;
    token.print();
  doitagain:;
    switch (statestack.top()) {
    case ParserState::Toplevel:
      switch (token.type) {
      case TOK_OPTION:
        statestack.push(ParserState::Option);
        break;
      case TOK_STOPWORD:
        statestack.push(ParserState::Stopword);
        break;
      case TOK_NAME:
        persist = std::get<std::string>(token.value);
        statestack.push(ParserState::Name);
        break;
      case TOK_ERROR:
      default:
        std::printf("It's all gone to whack\n");
        failing = true;
      }
      break;
    case ParserState::Option:
      if (token.type != TokenType::TOK_NAME) {
        std::printf("invalid token\n");
        failing = true;
        break;
      }
      persist = std::get<std::string>(token.value);
      statestack.push(ParserState::OptionName);
      break;
    case ParserState::OptionName:
      if (token.type != TokenType::TOK_BOOL) {
        std::printf("invalid token\n");
        failing = true;
        break;
      }
      gen_lexer_options[std::get<std::string>(persist)] =
          std::get<bool>(token.value);
      statestack.pop(); // OptionName
      statestack.pop(); // Option
      break;
    case ParserState::Stopword:
      if (token.type != TokenType::TOK_LITSTRING &&
          token.type != TokenType::TOK_FILESTRING) {
        statestack.pop(); // Stopword
        goto doitagain;
      }
      if (token.type == TokenType::TOK_FILESTRING) {
        failing = true;
        std::printf("file string not yet supported\n");
        break;
      }
      gen_lexer_stopwords.insert(std::get<std::string>(token.value));
      break;
    case ParserState::Name:
      if (token.type == TokenType::TOK_OPCONST) {
        // persist = std::get<std::string>(token.value);
        statestack.push(ParserState::Const);
        break;
      }
      if (token.type == TokenType::TOK_OPDEFINE) {
        // persist = std::get<std::string>(token.value);
        statestack.push(ParserState::Define);
        break;
      }
      if (token.type == TokenType::TOK_OPNORMAL) {
        // persist = std::get<std::string>(token.value);
        statestack.push(ParserState::Normal);
        break;
      }
      failing = true;
      std::printf("expected normalisation, const, or define, invalid token\n");
      break;
    case ParserState::Define: {
      if (token.type != TOK_REGEX) {
        failing = true;
        std::printf("expected regex, invalid token %s - %s\n",
                    reverse_token_type[token.type],
                    std::get<std::string>(token.value).c_str());
        break;
      }
      std::string name = std::get<std::string>(persist);
      if (name.size() == 0) {
        printf("huh?\n");
      }
      if (values.count(name) != 0) {
        failing = true;
        std::printf("symbol '%s' is being redefined\n", name.c_str());
        break;
      }
      Regexp reg = std::get<Regexp>(token.value);
      reg.resolve(values);
      values[name] = std::make_pair(SymbolType::Define, new Regexp{reg});
      statestack.pop(); // define
      statestack.pop(); // name
      break;
    }
    case ParserState::Const:
      printf("Not impl'd yet\n");
      break;
    case ParserState::Normal:
      printf("Not impl'd yet\n");
      break;
    }
  } while (!failing);
}

#define TEST
#ifdef TEST
int main() {
  NParser parser;
  parser.compile(R"(
  option lemmatise on
  stopword "test" "fest" "stopwords.list"
  # boo     :- "testt"
  # test    :- "43"
  # t       <= test
  boop    :: \d+
  POOP    :: {{boop}}-
  )");
}
#endif
