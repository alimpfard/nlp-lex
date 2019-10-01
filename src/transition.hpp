#pragma once
using EpsilonTransitionT = struct {};
using AnythingTransitionT = struct {};

template <typename TargetT, typename TransitionInputT> class Transition {
public:
  TargetT *target;
  TransitionInputT input;

  Transition(TargetT *a, TransitionInputT b) : target(a), input(b) {}
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
           (std::holds_alternative<char>(other.input) &&
            std::holds_alternative<char>(input) &&
            std::get<char>(input) == std::get<char>(other.input)) &&
           (std::holds_alternative<AnythingTransitionT>(other.input) &&
            std::holds_alternative<AnythingTransitionT>(input)) &&
           (std::holds_alternative<EpsilonTransitionT>(other.input) &&
            std::holds_alternative<EpsilonTransitionT>(input));
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
