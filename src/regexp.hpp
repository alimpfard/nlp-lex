#pragma once

#include <string>
#include <variant>

class NLexer;
class NFANode;
struct Regexp;

enum class SymbolType;

#include "parser.hpp"

enum RegexpType {
  Symbol,
  Alternative, // -> children
  Concat,      // -> children
  Nested,      // -> inner : Regexp*
  Literal,
  Escape,
  CharacterClass,
};

struct RepeatQuantifier {
  bool has_highbound;
  int lowbound, highbound;
};

struct Regexp {
  // here there be dragons
  RegexpType type;
  std::variant<std::string, char, Regexp *> inner;
  std::vector<Regexp *> children;

public:
  std::string str;

  bool plus, star, lazy;
  std::optional<RepeatQuantifier> repeat;

  bool operator==(const Regexp &other) const;

  Regexp concat(const Regexp &other);
  bool sanity_check(const NLexer &lexer);
  void resolve(
      const std::map<std::string,
                     std::pair<SymbolType, std::variant<std::string, Regexp *>>>
          values);

  Regexp(std::string str, RegexpType type, std::vector<Regexp *> children)
      : type(type), inner({}), children(children), plus(), star(), lazy(),
        str(str), repeat() {}

  Regexp(std::string str, RegexpType type, std::string inner)
      : type(type), inner(inner), plus(), star(), lazy(), str(str), repeat() {
    children = {};
  }

  Regexp(std::string str, RegexpType type, char inner)
      : type(type), inner(inner), plus(), star(), lazy(), str(str), repeat() {
    children = {};
  }

  Regexp(std::string str, RegexpType type, Regexp *inner)
      : type(type), inner(inner), plus(), star(), lazy(), str(str), repeat() {
    children = {};
  }

  std::string to_str() const;

  NFANode compile() const;
};
