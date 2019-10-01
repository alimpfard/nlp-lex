#pragma once
using EpsilonTransitionT = struct {};
using AnythingTransitionT = struct {};

template <typename TargetT, typename TransitionInputT> class Transition {
public:
  TargetT *target;
  TransitionInputT input;

  Transition(TargetT *a, TransitionInputT b) : target(a), input(b) {}
};
