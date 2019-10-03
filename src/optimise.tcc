#include <numeric>

template <typename T, typename C>
bool eqv(const std::set<T, C> &a, const std::set<T, C> &b) {
  if (a.size() != b.size())
    return false;
  std::printf("set b: {\n");
  for (auto var : b)
    var->print();
  std::printf("}\n");
  for (auto &x : a) {
    x->print();
    if (b.count(x) == 0) {
      std::printf("Nope\n");
      return false;
    }
  }
  return true;
}

template <typename T>
void reconstruct_forwards(NFANode<T> *p, bool clear = false) {
  if (p->start || clear) {
    for (auto x : p->get_outgoing_transitions(true))
      reconstruct_forwards(x->target, clear || p->start);
    p->incoming_transitions.clear();
  }
  for (auto q : p->get_input_end())
    for (auto x : q->outgoing_transitions) {
      // reconstruct_forwards(x->target, false);
      std::printf(
          "%s(%p) ",
          (q->state_info.value_or(q->named_rule.value_or("<unknown>"))).c_str(),
          q);
      x->print();
      x->target->incoming_transitions.insert(
          new typename std::remove_pointer<
              typename std::remove_reference<typename decltype(
                  x->target->incoming_transitions)::value_type>::type>::type{
              q, x->input});
    }
}

template <typename T> void NFANode<T>::optimise() {
  constexpr auto is_self_referencial = [](NFANode<T> *node) {
    for (auto tr : node->get_outgoing_transitions())
      if (tr->target == node)
        return true;
    return false;
  };
  constexpr auto has_nonepsilon_inc_transition = [](NFANode<T> *node) {
    for (auto inpend : node->get_input_end())
      for (auto tr : inpend->incoming_transitions)
        if (!std::holds_alternative<EpsilonTransitionT>(tr->input))
          return true;
    return false;
  };
  if (start)
    reconstruct_forwards(this);

  std::vector<std::pair<typename decltype(outgoing_transitions)::value_type,
                        NFANode<T> *>>
      outgoing_transitions_additions;
  std::set<typename decltype(outgoing_transitions)::value_type>
      outgoing_transitions_deletions;
  // short meaningless epsilon-transitions
  // /*
  for (auto it = outgoing_transitions.begin();
       it != outgoing_transitions.end();) {
    bool deld = false;
    (*it)->target->optimise();
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
          std::printf("found such transition as ");
          vs->print();
          ss.erase(ps);
        } else {
          std::printf("failed to find such transition as ");
          vs->print();
          // std::printf("in set: {\n");
          // for (auto s : ss)
          //   s->print();
          // std::printf("}\n");
        }
        delete vs;
        outgoing_transitions_additions.push_back({tr, node});
      }
      std::printf("wiped %p from existence\n", node);
      it = erase_transition_it(it);
      deld = true;
    dontdelete:;
    }

    if (!deld)
      ++it;
  }
  for (auto &[o, node] : outgoing_transitions_additions) {
    // add the wayward transitions
    add_transition(o);
  }
  outgoing_transitions_additions.clear();
  if (start)
    reconstruct_forwards(this);
  // */
  // unify equivalent states
  // /*
  std::set<NFANode<T> *> delds;
  // for (auto iss : get_input_end()) {
  //   for (auto it = iss->incoming_transitions.begin();
  //        it != iss->incoming_transitions.end();) {
  //     // if two states have the same outgoing transitions, we can safely
  //     unify
  //     // them
  //
  //     if (delds.count((*it)->target) != 0) {
  //       it++;
  //       continue;
  //     }
  //     for (auto it2 = iss->incoming_transitions.begin();
  //          it2 != iss->incoming_transitions.end(); it2++) {
  //       if (*it2 == *it)
  //         continue;
  //       auto av = (*it)->target->get_outgoing_transitions();
  //       auto av2 = (*it2)->target->get_outgoing_transitions();
  //       // if (av.size() == 0)
  //       //   goto dontdelete_1;
  //
  //       if (!eqv(av, av2))
  //         goto dontdelete_1;
  //       // *it->target and *it2->target are equivalent, keep *it->target and
  //       // disconnect *it2 move all incoming transitions from it2 to it
  //       // it -> A
  //       // it2 -> B
  //       // iss -> <C>
  //       for (auto bin : (*it2)->target->incoming_transitions) {
  //         auto eout = bin->target->get_outgoing_transitions();
  //         for (auto eo : eout) {
  //           if (eo->target == (*it2)->target) {
  //             eo->target = (*it)->target;
  //           }
  //           // add forward <e> to <*it->target> (A)
  //         }
  //       }
  //       // reconstruct_forwards((*it)->target, true);
  //       std::printf("wiped %p from existence through %p, going into %p\n",
  //                   (*it2)->target, (*it)->target, *iss);
  //       delds.insert((*it2)->target);
  //     dontdelete_1:;
  //     }
  //     it++;
  //   }
  //   for (auto &[o, node] : outgoing_transitions_additions) {
  //     // add the wayward transitions
  //     add_transition(o);
  //   }
  //   outgoing_transitions_additions.clear();
  //   delds.clear();
  // }
  if (start)
    reconstruct_forwards(this);
  decltype(get_outgoing_transitions()) rtr;
  for (auto b_candidate_tr : get_outgoing_transitions()) {
    if (delds.count(b_candidate_tr->target) > 0)
      continue;
    for (auto c_candidate_tr : get_outgoing_transitions()) {
      if (b_candidate_tr == c_candidate_tr)
        continue;
      if (!eqv(b_candidate_tr->target->incoming_transitions,
               c_candidate_tr->target->incoming_transitions))
        continue;
      auto b_candidate = b_candidate_tr->target;
      auto c_candidate = c_candidate_tr->target;
      for (auto ctr : c_candidate->get_outgoing_transitions())
        for (auto x : b_candidate->get_output_end())
          x->add_transition(ctr);
      rtr.insert(c_candidate_tr);
      delds.insert(c_candidate);
    }
  }
  for (auto tr : rtr)
    outgoing_transitions.erase(tr);
  if (start)
    reconstruct_forwards(this);
  // */
  /*
  for (auto it = outgoing_transitions.begin();
       it != outgoing_transitions.end();) {
    bool deld = false;
    // if two states have the same incoming transitions, or the same outgoing
    // transitions, they can be unified
    auto av = (*it)->target->incoming_transitions;
    if (av.size() == 0) {
      it++;
      continue;
    }
    std::set<NFANode<T> *> delds;
    for (auto iter = outgoing_transitions.begin();
         iter != outgoing_transitions.end(); ++iter) {
      if (*it == *iter)
        continue;
      if (delds.count((*iter)->target) != 0)
        continue;
      std::printf("'%s' == '%s'? ", (*it)->target->named_rule.value().c_str(),
                  (*iter)->target->named_rule.value().c_str());
      if (eqv(av, (*iter)->target->incoming_transitions)) {
        std::printf("yes\n");
        // they're the same
        auto og = (*iter)->target->get_outgoing_transitions();
        for (auto oi = og.begin(); oi != og.end(); ++oi) {
          auto &its = (*oi)->target->incoming_transitions;
          auto *ov = new typename std::remove_pointer<
              typename std::remove_reference<typename decltype(
                  (*iter)->target->incoming_transitions)::value_type>::type>::
              type{(*iter)->target, (*oi)->input};
          auto pp = its.find(ov);
          if (pp != its.end())
            (*pp)->target = this;
          else {
            std::printf("cannot seem to find any (2) ");
            ov->print();
          }
        }
        delds.insert((*iter)->target);
        outgoing_transitions_deletions.insert(*iter);
      } else
        std::printf("no\n");
    }
    ++it;
  }
  for (auto it = outgoing_transitions.begin();
       it != outgoing_transitions.end();) {
    if (outgoing_transitions_deletions.count(*it) > 0)
      it = outgoing_transitions.erase(it);
    else
      ++it;
  }
  outgoing_transitions_deletions.clear();
  // */
  // TODO: more optimisations
}

template <typename T> void PseudoNFANode<T>::optimise() {
  input_end->optimise();
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
    std::printf("in set: {\n");
    for (auto s : its)
      s->print();
    std::printf("}\n");
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
  outgoing_transitions.insert(tr);
}
