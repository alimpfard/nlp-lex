#pragma once

#include <functional>

enum EpsilonTransitionProperty : uint {
  Nothing = 0,
  ReadForward,
};

struct EpsilonTransitionT {
public:
  EpsilonTransitionProperty properties = EpsilonTransitionProperty::Nothing;
  bool operator==(const EpsilonTransitionT &other) const {
    return properties == other.properties;
  }
  bool operator<(const EpsilonTransitionT &other) const { return false; }
};

struct AnythingTransitionT {
  bool inverted;
  std::string values;
  bool operator==(const AnythingTransitionT &other) const {
    return other.inverted == inverted && other.values == values;
  }
  bool operator==(char other) const {
    return (values.find(other) != values.npos) ^ inverted;
  }
};

template <typename TargetT, typename TransitionInputT> class Transition {
public:
  TargetT *target;
  TransitionInputT input;

  Transition(TargetT *a, TransitionInputT b)
      : target(deep_input_end(a)), input(b) {}
  void print() {
    std::printf(
        "(-%s-> %s(%p))\n",
        std::holds_alternative<EpsilonTransitionT>(input)
            ? "<e>"
            : std::holds_alternative<AnythingTransitionT>(input)
                  ? "<A>"
                  : std::string{std::get<char>(input)}.c_str(),
        target->named_rule.value_or(target->state_info.value_or("???")).c_str(),
        target);
  }
};

template <typename NodeT, typename TransitionInputT> class CanonicalTransition {
public:
  NodeT *source, *target;
  TransitionInputT input;

  CanonicalTransition(NodeT *src, NodeT *dst, TransitionInputT input)
      : source(src), target(dst), input(input) {}

  bool
  operator==(const CanonicalTransition<NodeT, TransitionInputT> &other) const {
    return other.target == target && other.source == source &&
           ((std::holds_alternative<char>(other.input) &&
             std::holds_alternative<char>(input) &&
             std::get<char>(input) == std::get<char>(other.input)) ||
            (std::holds_alternative<char>(other.input) &&
             std::holds_alternative<AnythingTransitionT>(input) &&
             std::get<AnythingTransitionT>(input) ==
                 std::get<char>(other.input))) &&
           (std::holds_alternative<AnythingTransitionT>(other.input) &&
            std::holds_alternative<AnythingTransitionT>(input)) &&
           (std::holds_alternative<EpsilonTransitionT>(other.input) &&
            std::holds_alternative<EpsilonTransitionT>(input));
  }
};

template <typename NodeT>
class CanonicalTransition<NodeT, std::variant<char, EpsilonTransitionT>> {
public:
  NodeT *source, *target;
  std::variant<char, EpsilonTransitionT> input;

  CanonicalTransition(NodeT *src, NodeT *dst, decltype(input) input)
      : source(src), target(dst), input(input) {}

  bool operator==(
      const CanonicalTransition<NodeT, std::variant<char, EpsilonTransitionT>>
          &other) const {
    return other.target == target && other.source == source &&
           input == other.input;
  }
};

namespace std {

template <typename NodeT, typename TransitionInputT>
struct hash<CanonicalTransition<NodeT, TransitionInputT>> {
  std::size_t
  operator()(const CanonicalTransition<NodeT, TransitionInputT> &k) const {
    using std::hash;
    using std::size_t;
    using std::string;

    return ((hash<void *>()(k.source) ^ (hash<void *>()(k.target) << 1)) >> 1);
  }
};

} // namespace std
