#pragma once

#include <optional>
#include <set>
#include <variant>
#include <vector>

#include "transition.hpp"

template <typename StateInfoT> class NFANode;
template <typename StateInfoT> class DFANode;
template <typename K> struct NFANodePointerComparer;
template <typename K> struct DFANodePointerComparer;
template <typename K> struct TransitionPointerComparer;
template <typename T> NFANode<T> *deep_input_end(NFANode<T> *node);
template <typename T> NFANode<T> *deep_output_end(NFANode<T> *node);

static const AnythingTransitionT AnythingTransition{true, "\r\n"};

template <typename StateInfoT> class DFANode {
public:
  std::optional<StateInfoT> state_info;

  std::string
  gen_dot(std::set<DFANode<StateInfoT> *> nodes,
          std::unordered_set<CanonicalTransition<DFANode<StateInfoT>, char>>
              transitions);
  void aggregate_dot(
      std::set<DFANode<StateInfoT> *, DFANodePointerComparer<StateInfoT>>
          &nodes,
      std::set<DFANode<StateInfoT> *> &anodes,
      std::unordered_set<CanonicalTransition<DFANode<StateInfoT>, char>>
          &transitions);

  bool final, start, dirty = false, subexpr = false, reference_node = false;
  int max_opt_steps = 50;
  int opt_step = max_opt_steps;

  std::set<Transition<DFANode, char> *, TransitionPointerComparer<StateInfoT>>
      outgoing_transitions;
  std::set<Transition<DFANode, char> *, TransitionPointerComparer<StateInfoT>>
      /* reconstructed before optimisation */ incoming_transitions;

  DFANode<StateInfoT> *default_transition =
      nullptr; // where to go if nothing else matches
               // nullptr :- fail match and revert back to last final state

  std::optional<std::string> named_rule;
  DFANode(StateInfoT s) : state_info(s) {}
  DFANode()
      : state_info(), final(false), start(false), outgoing_transitions(),
        incoming_transitions(), named_rule() {}

  // virtual std::set<Transition<DFANode<StateInfoT>, char> *,
  //                  TransitionPointerComparer<StateInfoT>>
  // get_outgoing_transitions(bool inner = false);
  // void print();
  void print_dot();

  // virtual void optimise(std::set<DFANode<StateInfoT> *> visited, int step);
  // void optimise(std::set<DFANode<StateInfoT> *> visited);
  // typename std::set<Transition<DFANode, char> *,
  //                   TransitionPointerComparer<StateInfoT>>::iterator
  // erase_transition_it(
  //     typename std::set<Transition<DFANode, char> *,
  //                       TransitionPointerComparer<StateInfoT>>::iterator);
  void add_transition(Transition<DFANode, char> *);
  void default_transition_to(DFANode<StateInfoT> *node) {
    if (default_transition != nullptr)
      std::printf("[WARN] redefinition of the default transition for node %p "
                  "from %p to %p\n",
                  this, default_transition, node);
    default_transition = node;
  }
};

template <typename StateInfoT> class NFANode {
  static constexpr EpsilonTransitionT EpsilonTransition{};

public:
  std::optional<StateInfoT> state_info;

  std::string
  gen_dot(std::set<NFANode<StateInfoT> *> nodes,
          std::unordered_set<CanonicalTransition<
              NFANode<StateInfoT>,
              std::variant<char, EpsilonTransitionT, AnythingTransitionT>>>
              transitions);
  void aggregate_dot(
      std::set<NFANode<StateInfoT> *, NFANodePointerComparer<StateInfoT>>
          &nodes,
      std::set<NFANode<StateInfoT> *> &anodes,
      std::unordered_set<CanonicalTransition<
          NFANode<StateInfoT>,
          std::variant<char, EpsilonTransitionT, AnythingTransitionT>>>
          &transitions);

  bool final, start, dirty = false, subexpr = false, reference_node = false;
  int max_opt_steps = 50;
  int opt_step = max_opt_steps;
  std::set<Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                            AnythingTransitionT>> *,
           TransitionPointerComparer<StateInfoT>>
      outgoing_transitions;
  std::set<Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                            AnythingTransitionT>> *,
           TransitionPointerComparer<StateInfoT>>
      /* reconstructed before optimisation */ incoming_transitions;

  NFANode<StateInfoT> *default_transition =
      nullptr; // where to go if nothing else matches
               // nullptr :- fail match and revert back to last final state
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
  virtual void transition_to(NFANode<StateInfoT> *node, AnythingTransitionT c);
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
  virtual void optimise(std::set<NFANode<StateInfoT> *> visited, int step);
  void optimise(std::set<NFANode<StateInfoT> *> visited);
  typename std::set<Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                                     AnythingTransitionT>> *,
                    TransitionPointerComparer<StateInfoT>>::iterator
  erase_transition_it(typename std::set<
                      Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                                       AnythingTransitionT>> *,
                      TransitionPointerComparer<StateInfoT>>::iterator);
  void add_transition(Transition<NFANode, std::variant<char, EpsilonTransitionT,
                                                       AnythingTransitionT>> *);
  virtual void default_transition_to(NFANode<StateInfoT> *node) {
    node = deep_input_end(node);
    auto oe = get_output_end();
    for (auto oe : oe) {
      if (oe->default_transition != nullptr)
        std::printf("[WARN] redefinition of the default transition for node %p "
                    "from %p to %p\n",
                    oe, oe->default_transition, node);
      oe->default_transition = node;
    }
  }
  DFANode<std::set<NFANode<StateInfoT> *>> *to_dfa(); // call on root~
};

template <typename StateInfoT>
class PseudoNFANode : public NFANode<StateInfoT> {
public:
  NFANode<StateInfoT> *input_end, *output_end;

  PseudoNFANode(StateInfoT s, NFANode<StateInfoT> *in, NFANode<StateInfoT> *out)
      : NFANode<StateInfoT>(s), input_end(deep_input_end(in)),
        output_end(deep_output_end(out)) {}
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
  virtual void optimise(std::set<NFANode<StateInfoT> *> visited, int step);
};

template <typename T, typename C>
bool veqv(const std::set<T, C> &a, const std::set<T, C> &b) {
  if (a.size() != b.size())
    return false;
  for (auto &x : a) {
    // x->print();
    if (b.count(x) == 0) {
      return false;
    }
  }
  return true;
}

template <typename K> struct NFANodePointerComparer {
  bool eq(const NFANode<K> *a, const NFANode<K> *b) const {
    if (a == b)
      return true;
    if (dynamic_cast<const PseudoNFANode<K> *>(a) !=
        dynamic_cast<const PseudoNFANode<K> *>(b))
      return false;
    if (a->start == b->start && a->final == b->final && /*
        a->state_info == b->state_info && a->named_rule == b->named_rule*/
        1) {
      return a->default_transition == b->default_transition &&
             veqv(a->outgoing_transitions, b->outgoing_transitions);
    }
    return false;
  }
  bool operator()(const NFANode<K> *a, const NFANode<K> *b) const {

    // std::printf("state %p =? %p (%s =? %s) (%s =? %s)\n", a, b,
    //             a->named_rule.value_or("?").c_str(),
    //             b->named_rule.value_or("?").c_str(),
    //             a->state_info.value_or("?").c_str(),
    //             b->state_info.value_or("?").c_str());
    if (eq(a, b)) {
      return false;
    }
    return a < b;
  }
};

template <typename K> struct DFANodePointerComparer {
  bool eq(const DFANode<K> *a, const DFANode<K> *b) const {
    if (a == b)
      return true;
    if (a->start == b->start && a->final == b->final && /*
        a->state_info == b->state_info && a->named_rule == b->named_rule*/
        1) {
      return a->default_transition == b->default_transition &&
             veqv(a->outgoing_transitions, b->outgoing_transitions);
    }
    return false;
  }
  bool operator()(const DFANode<K> *a, const DFANode<K> *b) const {

    // std::printf("state %p =? %p (%s =? %s) (%s =? %s)\n", a, b,
    //             a->named_rule.value_or("?").c_str(),
    //             b->named_rule.value_or("?").c_str(),
    //             a->state_info.value_or("?").c_str(),
    //             b->state_info.value_or("?").c_str());
    if (eq(a, b)) {
      return false;
    }
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
    // if (NFANodePointerComparer<K>().eq(a->target, b->target) &&
    //     a->input == b->input)
    //   return true;
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
  bool eq(const Transition<DFANode<K>, char> *a,
          const Transition<DFANode<K>, char> *b) const {
    if (a == b)
      return true;
    if (a == nullptr || b == nullptr)
      return false;
    // if (NFANodePointerComparer<K>().eq(a->target, b->target) &&
    //     a->input == b->input)
    //   return true;
    return false;
  }
  bool operator()(const Transition<DFANode<K>, char> *a,
                  const Transition<DFANode<K>, char> *b) const {
    if (eq(a, b))
      return false;

    return a < b;
  }
};

template <typename T> NFANode<T> *deep_input_end(NFANode<T> *node) {
  if (dynamic_cast<PseudoNFANode<T> *>(node) == nullptr)
    return node;
  return deep_input_end(dynamic_cast<PseudoNFANode<T> *>(node)->input_end);
}

template <typename T> NFANode<T> *deep_output_end(NFANode<T> *node) {
  if (dynamic_cast<PseudoNFANode<T> *>(node) == nullptr)
    return node;
  return deep_output_end(dynamic_cast<PseudoNFANode<T> *>(node)->output_end);
}

template <typename T> DFANode<T> *deep_input_end(DFANode<T> *node) {
  return node;
}

template <typename T> DFANode<T> *deep_output_end(DFANode<T> *node) {
  return node;
}
