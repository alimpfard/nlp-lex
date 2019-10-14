#include <numeric>

#define NO_INCOMING_UNIFY 0
#define NO_EPSILON_SHORT 0
#define NO_EPSILON_SELF_REF_REMOVE 0

template <typename T, typename C>
bool eqv(const std::set<T, C> &a, const std::set<T, C> &b) {
  if (a.size() != b.size())
    return false;
  // std::printf("set b: {\n");
  // for (auto var : b)
  // var->print();
  // std::printf("}\n");
  for (auto &x : a) {
    // x->print();
    auto bb = false;
    for (auto y : b)
      if (x->target == y->target && x->input == y->input) {
        bb = true;
        break;
      }
    if (!bb) {
      // std::printf("Nope\n");
      return false;
    }
  }
  return true;
}

template <typename T>
void reconstruct_forwards(NFANode<T> *p, std::set<NFANode<T> *> pn,
                          bool clear = false) {
  if (pn.count(p))
    return;
  pn.insert(p);
  if (p->start || clear) {
    for (auto x : p->get_outgoing_transitions(true))
      reconstruct_forwards(x->target, pn, clear || p->start);
    p->incoming_transitions.clear();
  }
  for (auto q : p->get_input_end())
    for (auto x : q->outgoing_transitions) {
      // reconstruct_forwards(x->target, false);
      // std::printf(
      //     "%s(%p) ",
      //     (q->state_info.value_or(q->named_rule.value_or("<unknown>"))).c_str(),
      //     q);
      // x->print();
      x->target->incoming_transitions.insert(
          new typename std::remove_pointer<
              typename std::remove_reference<typename decltype(
                  x->target->incoming_transitions)::value_type>::type>::type{
              q, x->input});
    }
}
template <typename T>
constexpr bool references(NFANode<T> *node, NFANode<T> *other,
                          std::set<NFANode<T> *> visited = {}) {
  if (visited.count(node))
    return false;
  visited.insert(node);
  for (auto tr : node->get_outgoing_transitions()) {
    if (tr->target == other)
      return true;
    if (std::holds_alternative<EpsilonTransitionT>(tr->input)) {
      return references(tr->target, other, visited);
    }
  }
  return false;
};
template <typename T> constexpr bool is_self_referencial(NFANode<T> *node) {
  for (auto tr : node->get_outgoing_transitions()) {
    if (std::holds_alternative<EpsilonTransitionT>(tr->input)) {
      return references(tr->target, node);
    } else if (tr->target == node)
      return true;
  }
  return false;
};

template <typename T>
void NFANode<T>::optimise(std::set<NFANode<T> *> visited) {
  for (int i = 0; i < max_opt_steps; ++i) {
    this->optimise(visited, i);
  }
}
template <typename T>
void NFANode<T>::optimise(std::set<NFANode<T> *> visited, int step) {
  if (visited.count(this)) {
    if (!dirty || !step)
      return;
  }
  dirty = false;
  constexpr auto has_nonepsilon_inc_transition = [](NFANode<T> *node) {
    for (auto inpend : node->get_input_end())
      for (auto tr : inpend->incoming_transitions)
        if (!std::holds_alternative<EpsilonTransitionT>(tr->input))
          return true;
    return false;
  };
  if (start)
    reconstruct_forwards(this, {});

  std::vector<std::pair<typename decltype(outgoing_transitions)::value_type,
                        NFANode<T> *>>
      outgoing_transitions_additions;
  std::set<typename decltype(outgoing_transitions)::value_type>
      outgoing_transitions_deletions;
  // std::printf("visited %p\n", this);
  visited.insert(this);
  if (!NO_EPSILON_SELF_REF_REMOVE) {
    // /*
    // std::printf("remove empty transitions to self {\n");
    for (auto it = outgoing_transitions.begin();
         it != outgoing_transitions.end();) {
      bool deld = false;
      (*it)->target->optimise(visited, step);
      if (std::holds_alternative<EpsilonTransitionT>((*it)->input) &&
          (*it)->target == this) {
        it = erase_transition_it(it);
        deld = true;
        dirty = true;
      }
      if (!deld)
        ++it;
    }
  } // NO_EPSILON_SELF_REF_REMOVE
    // std::printf("remove empty transitions to self }\n");
    // short meaningless epsilon-transitions
  if (!NO_EPSILON_SHORT) {
    if (start)
      reconstruct_forwards(this, {});
    // std::printf("short unnessary epsilon transitions {\n");
    for (auto it = outgoing_transitions.begin();
         it != outgoing_transitions.end();) {
      bool deld = false;
      (*it)->target->optimise(visited, step);
      if (std::holds_alternative<EpsilonTransitionT>((*it)->input)) {
        auto node = (*it)->target;
        if (is_self_referencial(node) || has_nonepsilon_inc_transition(node))
          goto dontdelete; // self-ref loops cannot be optimised away
        for (auto tr : node->get_outgoing_transitions()) {
          auto ss = tr->target->incoming_transitions;
          auto *vs = new typename std::remove_reference<decltype(*tr)>::type{
              node, tr->input};
          auto ps = ss.find(vs);
          if (ps != ss.end()) {
            // std::printf("found such transition as ");
            // vs->print();
            ss.erase(ps);
          } else {
            std::printf("failed to find such transition as ");
            vs->print();
          }
          delete vs;
          outgoing_transitions_additions.push_back({tr, node});
        }
        // std::printf("wiped %p from existence\n", node);
        it = erase_transition_it(it);
        if (node->final) {
          final = true;
          // if the node is final, we'd like to preserve its name as well
          state_info = node->state_info;
          named_rule = node->named_rule;
        }
        if (node->default_transition) {
          if (default_transition &&
              default_transition != node->default_transition) {
            std::printf("overriding default transition %p to %p on %p",
                        default_transition, node->default_transition, this);
          }
          default_transition = node->default_transition;
        }
        deld = true;
      dontdelete:;
      }

      if (!deld)
        ++it;
    }
    for (auto &[o, node] : outgoing_transitions_additions) {
      // add the wayward transitions
      add_transition(o);
      node->dirty = true;
      dirty = true;
    }
    outgoing_transitions_additions.clear();
    // std::printf("short unnessary epsilon transitions }\n");

  } // NO_EPSILON_SHORT
    // unify equivalent states
  if (!NO_INCOMING_UNIFY) {
    // std::printf("merge leading transitions %p {\n", this);
    std::set<NFANode<T> *> delds;
    if (start)
      reconstruct_forwards(this, {});
    decltype(get_outgoing_transitions()) rtr;
    for (auto b_candidate_tr : get_outgoing_transitions()) {
      // std::printf("\tconsidering outgoing transition (to %p) {\n",
      //             b_candidate_tr->target);
      if (delds.count(b_candidate_tr->target) > 0) {
        // std::printf("was already deleted ");
        // b_candidate_tr->print();
        for (auto c : b_candidate_tr->target->get_outgoing_transitions())
          c->target->optimise(visited, step);
        continue;
      }
      for (auto c_candidate_tr : get_outgoing_transitions()) {
        if (b_candidate_tr == c_candidate_tr)
          continue;
        if (b_candidate_tr->target == c_candidate_tr->target)
          continue;
        // if (b_candidate_tr->target->start != c_candidate_tr->target->start ||
        //     b_candidate_tr->target->final != c_candidate_tr->target->final)
        //   continue;
        if (!eqv(b_candidate_tr->target->incoming_transitions,
                 c_candidate_tr->target->incoming_transitions))
          continue;
        auto b_candidate = b_candidate_tr->target;
        auto c_candidate = c_candidate_tr->target;
        for (auto ctr : c_candidate->get_outgoing_transitions())
          for (auto x : b_candidate->get_output_end()) {
            x->add_transition(ctr);
            x->dirty = true;
          }
        rtr.insert(c_candidate_tr);
        delds.insert(c_candidate);
        if (c_candidate->final) {
          b_candidate->final = true;
          b_candidate->state_info =
              c_candidate->state_info; // XXX this might clash
          b_candidate->named_rule = c_candidate->named_rule;
        }
        if (c_candidate->default_transition) {
          if (b_candidate->default_transition &&
              b_candidate->default_transition !=
                  c_candidate->default_transition) {
            std::printf("overriding default transition %p to %p on %p",
                        b_candidate->default_transition,
                        c_candidate->default_transition, b_candidate);
          }
          b_candidate->default_transition = c_candidate->default_transition;
        }
      }
      // std::printf("\tconsidering outgoing transition %p }\n",
      // b_candidate_tr);
    }
    for (auto tr : rtr) {
      outgoing_transitions.erase(tr);
      dirty = true;
    }
    // std::printf("merge leading transitions }\n");
    if (start)
      reconstruct_forwards(this, {});
  } // NO_INCOMING_UNIFY
  /* TODO OUTGOING_UNIFY
   */
  // TODO: more optimisations
}

template <typename T>
void PseudoNFANode<T>::optimise(std::set<NFANode<T> *> visited, int step) {
  visited.insert(this);
  input_end->optimise(visited, step);
}

template <typename T>
typename std::set<Transition<NFANode<T>, std::variant<char, EpsilonTransitionT,
                                                      AnythingTransitionT>> *,
                  TransitionPointerComparer<T>>::iterator
NFANode<T>::erase_transition_it(
    typename std::set<
        Transition<NFANode<T>, std::variant<char, EpsilonTransitionT,
                                            AnythingTransitionT>> *,
        TransitionPointerComparer<T>>::iterator it) {
  // remove forward ref
  Transition<NFANode<T>,
             std::variant<char, EpsilonTransitionT, AnythingTransitionT>>
      tr{this, (*it)->input};
  auto &its = (*it)->target->incoming_transitions;
  auto pos = its.find(&tr);
  if (pos == its.end()) {
    std::printf("couldn't find forward-pointing transition");
    tr.print();
    // std::printf("in set: {\n");
    // for (auto s : its)
    // s->print();
    // std::printf("}\n");
  } else {
    its.erase(pos);
  }
  return outgoing_transitions.erase(it);
}
template <typename T>
void NFANode<T>::add_transition(
    Transition<NFANode<T>,
               std::variant<char, EpsilonTransitionT, AnythingTransitionT>>
        *tr) {
  Transition<NFANode<T>,
             std::variant<char, EpsilonTransitionT, AnythingTransitionT>>
      tbr{this, tr->input};
  tr->target->incoming_transitions.insert(new decltype(tbr){tbr});
  deep_output_end(this)->outgoing_transitions.insert(tr);
}
