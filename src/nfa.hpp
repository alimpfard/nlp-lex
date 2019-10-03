#pragma once

#include <optional>
#include <set>
#include <variant>
#include <vector>

#include "transition.hpp"

template <typename K> struct NFANodePointerComparer;
template <typename K> struct TransitionPointerComparer;

template <typename StateInfoT> class NFANode {
  static constexpr EpsilonTransitionT EpsilonTransition{};
  static constexpr AnythingTransitionT AnythingTransition{};

public:
  std::optional<StateInfoT> state_info;

  std::string gen_dot(
      std::set<NFANode<StateInfoT> *, NFANodePointerComparer<StateInfoT>> nodes,
      std::unordered_set<CanonicalTransition<
          NFANode<StateInfoT>,
          std::variant<char, EpsilonTransitionT, AnythingTransitionT>>>
          transitions);
  void aggregate_dot(
      std::set<NFANode<StateInfoT> *, NFANodePointerComparer<StateInfoT>>
          &nodes,
      std::unordered_set<CanonicalTransition<
          NFANode<StateInfoT>,
          std::variant<char, EpsilonTransitionT, AnythingTransitionT>>>
          &transitions);

  bool final, start;
  std::set<Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                            AnythingTransitionT>> *,
           TransitionPointerComparer<StateInfoT>>
      outgoing_transitions;
  std::set<Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                            AnythingTransitionT>> *,
           TransitionPointerComparer<StateInfoT>>
      /* reconstructed before optimisation */ incoming_transitions;
  std::optional<std::string> named_rule;
  NFANode(StateInfoT s) : state_info(s) {}
  NFANode()
      : state_info(), final(false), start(false), outgoing_transitions(),
        incoming_transitions(), named_rule() {}
  virtual std::vector<NFANode<StateInfoT> *> get_input_end() {
    std::vector<NFANode<StateInfoT> *> pv;
    pv.push_back(this);
    return pv;
  }
  virtual std::vector<NFANode<StateInfoT> *> get_output_end() {
    std::vector<NFANode<StateInfoT> *> pv;
    pv.push_back(this);
    return pv;
  }

  virtual void transition_to(NFANode<StateInfoT> *node, char c);
  virtual void epsilon_transition_to(NFANode<StateInfoT> *node);
  virtual void anything_transition_to(NFANode<StateInfoT> *node);
  virtual std::set<
      Transition<NFANode<StateInfoT>,
                 std::variant<char, EpsilonTransitionT, AnythingTransitionT>> *,
      TransitionPointerComparer<StateInfoT>>
  get_outgoing_transitions(bool inner = false);
  void print();
  void print_dot();

  NFANode<StateInfoT> *copy_if(bool leading);
  virtual NFANode<StateInfoT> *deep_copy();
  virtual void optimise();
  typename std::set<Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                                     AnythingTransitionT>> *,
                    TransitionPointerComparer<StateInfoT>>::iterator
  erase_transition_it(typename std::set<
                      Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                                       AnythingTransitionT>> *,
                      TransitionPointerComparer<StateInfoT>>::iterator);
  void add_transition(Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                                       AnythingTransitionT>> *);
};

template <typename StateInfoT>
class PseudoNFANode : public NFANode<StateInfoT> {
public:
  NFANode<StateInfoT> *input_end, *output_end;

  PseudoNFANode(StateInfoT s, NFANode<StateInfoT> *in, NFANode<StateInfoT> *out)
      : NFANode<StateInfoT>(s), input_end(in), output_end(out) {}
  PseudoNFANode()
      : NFANode<StateInfoT>(), input_end(nullptr), output_end(nullptr) {}
  virtual std::vector<NFANode<StateInfoT> *> get_input_end() {
    auto *p = dynamic_cast<PseudoNFANode *>(input_end);
    if (p == nullptr) {
      std::vector<NFANode<StateInfoT> *> pv;
      pv.push_back(input_end);
      return pv;
    } else
      return p->get_input_end();
  }
  virtual std::vector<NFANode<StateInfoT> *> get_output_end() {
    auto *p = dynamic_cast<PseudoNFANode *>(output_end);
    if (p == nullptr) {
      std::vector<NFANode<StateInfoT> *> pv;
      pv.push_back(output_end);
      return pv;
    } else
      return p->get_output_end();
  }
  virtual std::set<
      Transition<NFANode<StateInfoT>,
                 std::variant<char, EpsilonTransitionT, AnythingTransitionT>> *,
      TransitionPointerComparer<StateInfoT>>
  get_outgoing_transitions(bool inner = false);
  virtual NFANode<StateInfoT> *deep_copy();
  virtual void optimise();
};

template <typename K> struct NFANodePointerComparer {
  bool eq(const NFANode<K> *a, const NFANode<K> *b) const {
    if (a == b)
      return true;
    if (dynamic_cast<const PseudoNFANode<K> *>(a) !=
        dynamic_cast<const PseudoNFANode<K> *>(b))
      return false;
    if (a->start == b->start && a->final == b->final &&
        a->state_info == b->state_info && a->named_rule == b->named_rule) {
      if (a->outgoing_transitions.size() != b->outgoing_transitions.size())
        return false;
      for (int i = 0; i < a->outgoing_transitions.size(); i++)
        if (!(eq(a->outgoing_transitions[i]->target,
                 b->outgoing_transitions[i]->target) &&
              a->outgoing_transitions[i]->input ==
                  b->outgoing_transitions[i]->input))
          return false;
      return true;
    }
    return false;
  }
  bool operator()(const NFANode<K> *a, const NFANode<K> *b) const {
    // if (eq(a, b)) {
    //   std::printf("state %p = %p (%s = %s) (%s = %s)?\n", a, b,
    //               a->named_rule.value_or("?").c_str(),
    //               b->named_rule.value_or("?").c_str(),
    //               a->state_info.value_or("?").c_str(),
    //               b->state_info.value_or("?").c_str());
    //   return false;
    // }
    return a < b;
  }
};
template <typename K> struct TransitionPointerComparer {
  bool
  eq(const Transition<NFANode<K>, std::variant<char, EpsilonTransitionT,
                                               AnythingTransitionT>> *a,
     const Transition<NFANode<K>, std::variant<char, EpsilonTransitionT,
                                               AnythingTransitionT>> *b) const {
    if (a == b)
      return true;
    if (a == nullptr || b == nullptr)
      return false;
    if (a->target == b->target && a->input == b->input)
      return true;
    return false;
  }
  bool
  operator()(const Transition<NFANode<K>, std::variant<char, EpsilonTransitionT,
                                                       AnythingTransitionT>> *a,
             const Transition<NFANode<K>, std::variant<char, EpsilonTransitionT,
                                                       AnythingTransitionT>> *b)
      const {
    if (eq(a, b))
      return false;

    return a < b;
  }
};
