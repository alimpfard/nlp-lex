#pragma once

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stack>
#include <unordered_set>
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

struct SymbolDebugInformation {
  SymbolType type;
  int lineno;
  int offset;
  int length;
  std::string name;
  std::string definition_string;
};

class NParser {
  std::map<std::string, std::tuple<SymbolType, SymbolDebugInformation,
                                   std::variant<std::string, Regexp *>>>
      values;
  std::stack<ParserState> statestack;

  std::set<std::string> find_leaf_rules() const;

  std::map<std::string, bool> gen_lexer_options;
  std::set<std::string> gen_lexer_stopwords;
  std::map<char, char> gen_lexer_normalisations;

public:
  std::unique_ptr<NLexer> lexer;
  NFANode<std::string> compile(std::string code);
  NFANode<std::string> compile();
  void repl_feed(std::string code);
  void parse();
};