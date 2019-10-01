#pragma once

#include <optional>
#include <variant>
#include <vector>

#include "transition.hpp"

template <typename StateInfoT> class NFANode {
  static constexpr EpsilonTransitionT EpsilonTransition{};
  static constexpr AnythingTransitionT AnythingTransition{};

  std::optional<StateInfoT> state_info;
  std::vector<Transition<
      NFANode, std::variant<char, EpsilonTransitionT, AnythingTransitionT>>>
      outgoing_transitions;

public:
  NFANode(StateInfoT s) : state_info(s) {}
  virtual NFANode<StateInfoT> *get_input_end() { return this; }
  virtual NFANode<StateInfoT> *get_output_end() { return this; }

  void transition_to(NFANode<StateInfoT> *node, char c);
  void epsilon_transition_to(NFANode<StateInfoT> *node);
  void anything_transition_to(NFANode<StateInfoT> *node);
  void remove_transitions_to(NFANode<StateInfoT> *node);
  void remove_transition(
      Transition<NFANode<StateInfoT>,
                 std::variant<char, EpsilonTransitionT, AnythingTransitionT>>
          transition);
  void print();
};

template <typename StateInfoT>
class PseudoNFANode : public NFANode<StateInfoT> {
  NFANode<StateInfoT> *input_end, *output_end;

public:
  PseudoNFANode(StateInfoT s, NFANode<StateInfoT> *in, NFANode<StateInfoT> *out)
      : NFANode<StateInfoT>(s), input_end(in), output_end(out) {}
  virtual NFANode<StateInfoT> *get_input_end() { return input_end; }
  virtual NFANode<StateInfoT> *get_output_end() { return output_end; }
};
