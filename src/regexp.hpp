#pragma once

#include <string>
#include <variant>

class NLexer;
class NFA;

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

  Regexp concat(const Regexp &other);
  bool sanity_check(const NLexer &lexer);

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

  NFA compile() const;
};
