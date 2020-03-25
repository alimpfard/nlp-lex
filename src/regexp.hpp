#pragma once

#include <string>
#include <variant>

class NLexer;

template <typename T> class NFANode;

struct Regexp;

enum class SymbolType;
struct SymbolDebugInformation;

#include "debug.hpp"
#include "parser.hpp"

enum RegexpType {
  Symbol,
  Alternative, // -> children
  Concat,      // -> children
  Nested,      // -> inner : Regexp*
  Dot,
  Literal,
  Escape,
  CharacterClass,
  Assertion,
  SubExprCall,
  Code, // inner : string,
  Backreference,
};

struct RepeatQuantifier {
  bool has_highbound;
  int lowbound, highbound;
};

class Regexp {
public:
  // here there be dragons
  RegexpType type;
  CUDebugInformation debug_info;
  std::variant<std::string, char, Regexp *, std::vector<RegexpAssertion>> inner;
  std::vector<Regexp *> children;

  bool is_leaf = false;
  bool was_reference = false;
  std::optional<std::string> referenced_symbol, named_rule;

  std::string str;

  bool plus = false, star = false, lazy = false, store = false;
  int index = 0;        // applies for nested and backref (escape)
  int subexprcall = -1; // applies for SubExprCall

  std::optional<RepeatQuantifier> repeat;

  Regexp &set_is_leaf(bool il) {
    is_leaf = il;
    return *this;
  }
  Regexp *set_is_leaf_p(bool il) {
    is_leaf = il;
    return this;
  }
  void mark_leaves();

  bool operator==(const Regexp &other) const;

  Regexp concat(const Regexp &other);
  bool sanity_check(const NLexer &lexer);
  void resolve(const std::map<std::string,
                              std::tuple<SymbolType, SymbolDebugInformation,
                                         std::variant<std::string, Regexp *>>>
                   values);

  Regexp(std::string str, RegexpType type, std::vector<Regexp *> children,
         CUDebugInformation dbg)
      : type(type), inner({}), children(children), str(str), debug_info(dbg) {}

  Regexp(std::string str, RegexpType type, std::string inner,
         CUDebugInformation dbg)
      : type(type), inner(inner), str(str), debug_info(dbg) {
    children = {};
  }

  Regexp(std::string str, RegexpType type, char inner, CUDebugInformation dbg)
      : type(type), inner(inner), str(str), debug_info(dbg) {
    children = {};
  }

  Regexp(std::string str, RegexpType type, Regexp *inner,
         CUDebugInformation dbg)
      : type(type), inner(inner), str(str), debug_info(dbg) {
    children = {};
  }

  Regexp(std::string str, RegexpType type, std::vector<RegexpAssertion> inner,
         CUDebugInformation dbg)
      : type(type), inner(inner), str(str), debug_info(dbg) {
    children = {};
  }

  std::string to_str() const;
  std::string mangle() const;

  NFANode<std::string> *
  transform_by_quantifiers(NFANode<std::string> *node) const;

  NFANode<std::string> *
  compile(std::multimap<const Regexp *, NFANode<std::string> *> &cache,
          NFANode<std::string> *parent, std::string path, bool &leading,
          bool nopath = false) const;
};
