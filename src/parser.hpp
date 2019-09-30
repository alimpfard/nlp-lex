#pragma once

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stack>
#include <utility>
#include <variant>
#include <vector>

#include "nfa.hpp"
#include "regexp.hpp"

enum class SymbolType { Const, Define };
enum class ParserState {
  Toplevel = 0,
  // Option
  Option,
  OptionName,
  // Stopword
  Stopword,
  // Name
  Name,
  Define,
  Const,
  Normal,
};

class NParser {
  std::unique_ptr<NLexer> lexer;
  std::map<std::string,
           std::pair<SymbolType, std::variant<std::string, Regexp *>>>
      values;
  NFANode compile();
  std::stack<ParserState> statestack;

  std::map<std::string, bool> gen_lexer_options;
  std::set<std::string> gen_lexer_stopwords;
  std::map<char, char> gen_lexer_normalisations;

public:
  NFANode compile(std::string code);
};
