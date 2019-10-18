#pragma once

#include <string>
#include <variant>

class NLexer;

template <typename T> class NFANode;

struct Regexp;

enum class SymbolType;
struct SymbolDebugInformation;

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
};

struct RepeatQuantifier {
  bool has_highbound;
  int lowbound, highbound;
};

struct Regexp {
  // here there be dragons
  RegexpType type;
  std::variant<std::string, char, Regexp *, std::vector<RegexpAssertion>> inner;
  std::vector<Regexp *> children;

  bool is_leaf;
  bool was_reference;
  std::optional<std::string> referenced_symbol, named_rule;

public:
  std::string str;

  bool plus, star, lazy, store = false;
  int index = 0; // applies for nested and backref (escape)
  std::optional<RepeatQuantifier> repeat;

  bool operator==(const Regexp &other) const;

  Regexp concat(const Regexp &other);
  bool sanity_check(const NLexer &lexer);
  void resolve(const std::map<std::string,
                              std::tuple<SymbolType, SymbolDebugInformation,
                                         std::variant<std::string, Regexp *>>>
                   values);

  Regexp(std::string str, RegexpType type, std::vector<Regexp *> children)
      : type(type), inner({}), children(children), plus(), star(), lazy(),
        str(str), repeat(), is_leaf(true), was_reference(false) {}

  Regexp(std::string str, RegexpType type, std::string inner)
      : type(type), inner(inner), plus(), star(), lazy(), str(str), repeat(),
        is_leaf(true), was_reference(false) {
    children = {};
  }

  Regexp(std::string str, RegexpType type, char inner)
      : type(type), inner(inner), plus(), star(), lazy(), str(str), repeat(),
        is_leaf(true), was_reference(false) {
    children = {};
  }

  Regexp(std::string str, RegexpType type, Regexp *inner)
      : type(type), inner(inner), plus(), star(), lazy(), str(str), repeat(),
        is_leaf(true), was_reference(false) {
    children = {};
  }

  Regexp(std::string str, RegexpType type, std::vector<RegexpAssertion> inner)
      : type(type), inner(inner), plus(), star(), lazy(), str(str), repeat(),
        is_leaf(true), was_reference(false) {
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
