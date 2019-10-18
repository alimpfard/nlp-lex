#define TEST

#include "parser.hpp"
#include "codegen.hpp"
#include "lexer.hpp"
#include "optimise.tcc"

#include <array>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <stdarg.h>
#include <thread>

#include <algorithm>
#include <execution>

#include "vm.hpp"

#include "termdisplay.hpp"
#include <mutex>

constexpr EpsilonTransitionT EpsilonTransition{};
Display::SingleLineTermStatus slts;

NFANode<std::string> *NParser::compile(std::string code) {
  lexer = std::make_unique<NLexer>(code);
  return compile();
}

void NParser::repl_feed(std::string code) {
  lexer->source = code;
  lexer->source_p = strdup(code.c_str());
  lexer->lineno++;
  lexer->offset = 0;
}

void NParser::parse() {
  statestack = {};
  statestack.push(ParserState::Toplevel);
  bool failing = false;
  std::variant<std::string, int> persist;
  do {
    Token token = lexer->next();
    if (token.type == TOK_EOF)
      break;
    token.print();
  doitagain:;
    switch (statestack.top()) {
    case ParserState::Toplevel:
      switch (token.type) {
      case TOK_OPTION:
        statestack.push(ParserState::Option);
        break;
      case TOK_STOPWORD:
        statestack.push(ParserState::Stopword);
        break;
      case TOK_IGNORE:
        statestack.push(ParserState::Ignore);
        break;
      case TOK_NAME:
        persist = std::get<std::string>(token.value);
        statestack.push(ParserState::Name);
        break;
      case TOK_ERROR:
      default:
        std::printf("It's all gone to whack\n");
        failing = true;
      }
      break;
    case ParserState::Option:
      if (token.type != TokenType::TOK_NAME) {
        std::printf("invalid token\n");
        failing = true;
        break;
      }
      persist = std::get<std::string>(token.value);
      statestack.push(ParserState::OptionName);
      break;
    case ParserState::OptionName:
      if (token.type != TokenType::TOK_BOOL) {
        std::printf("invalid token\n");
        failing = true;
        break;
      }
      gen_lexer_options[std::get<std::string>(persist)] =
          std::get<bool>(token.value);
      statestack.pop(); // OptionName
      statestack.pop(); // Option
      break;
    case ParserState::Ignore:
      if (token.type == TokenType::TOK_CBRAC) {
        statestack.pop();
        break;
      }
      if (token.type != TokenType::TOK_NAME) {
        statestack.pop();
        goto doitagain;
      }
      gen_lexer_ignores.insert(std::get<std::string>(token.value));
      break;
    case ParserState::Stopword:
      if (token.type != TokenType::TOK_LITSTRING &&
          token.type != TokenType::TOK_FILESTRING) {
        statestack.pop(); // Stopword
        goto doitagain;
      }
      if (token.type == TokenType::TOK_FILESTRING) {
        failing = true;
        std::printf("file string not yet supported\n");
        break;
      }
      gen_lexer_stopwords.insert(std::get<std::string>(token.value));
      break;
    case ParserState::Name:
      if (token.type == TokenType::TOK_OPCONST) {
        // persist = std::get<std::string>(token.value);
        statestack.push(ParserState::Const);
        break;
      }
      if (token.type == TokenType::TOK_OPDEFINE) {
        // persist = std::get<std::string>(token.value);
        statestack.push(ParserState::Define);
        break;
      }
      if (token.type == TokenType::TOK_OPNORMAL) {
        // persist = std::get<std::string>(token.value);
        statestack.push(ParserState::Normal);
        break;
      }
      failing = true;
      std::printf("expected normalisation, const, or define, invalid token\n");
      break;
    case ParserState::Define: {
      if (token.type != TOK_REGEX) {
        failing = true;
        std::printf("expected regex, invalid token %s - %s\n",
                    reverse_token_type[token.type],
                    std::get<std::string>(token.value).c_str());
        break;
      }
      std::string name = std::get<std::string>(persist);
      if (name.size() == 0) {
        printf("huh?\n");
      }
      if (values.count(name) != 0) {
        failing = true;
        auto entry = std::get<1>(values[name]);
        std::printf(
            "symbol '%s' has been previously defined at line %d, offset %d\n",
            name.c_str(), entry.lineno, entry.offset);
        break;
      }
      Regexp reg = std::get<Regexp>(token.value);
      reg.resolve(values);
      reg.named_rule = name;
      values[name] = std::make_tuple<SymbolType, SymbolDebugInformation,
                                     std::variant<std::string, Regexp *>>(
          SymbolType::Define,
          {SymbolType::Define, token.lineno, token.offset, token.length, name,
           reg.str},
          new Regexp{reg});
      statestack.pop(); // define
      statestack.pop(); // name
      break;
    }
    case ParserState::Const: {
      if (token.type == TOK_REGEX) {
        std::string name = std::get<std::string>(persist);
        if (name.size() == 0) {
          printf("huh?\n");
        }
        if (values.count(name) != 0) {
          failing = true;
          auto entry = std::get<1>(values[name]);
          std::printf(
              "symbol '%s' has been previously defined at line %d, offset %d\n",
              name.c_str(), entry.lineno, entry.offset);
          break;
        }
        Regexp reg = std::get<Regexp>(token.value);
        reg.resolve(values);
        reg.named_rule = name;
        values[name] = std::make_tuple<SymbolType, SymbolDebugInformation,
                                       std::variant<std::string, Regexp *>>(
            SymbolType::Const,
            {SymbolType::Const, token.lineno, token.offset, token.length, name,
             reg.str},
            new Regexp{reg});
        statestack.pop(); // const
        statestack.pop(); // name
        break;
      } else if (token.type != TOK_LITSTRING && token.type != TOK_FILESTRING) {
        failing = true;
        std::printf("expected a string, invalid token %s - %s\n",
                    reverse_token_type[token.type],
                    std::get<std::string>(token.value).c_str());
        break;
      }
      std::string name = std::get<std::string>(persist);
      if (name.size() == 0) {
        printf("huh?\n");
      }
      if (values.count(name) != 0) {
        failing = true;
        auto entry = std::get<1>(values[name]);
        std::printf(
            "symbol '%s' has been previously defined at line %d, offset %d\n",
            name.c_str(), entry.lineno, entry.offset);
        break;
      }
      std::string s = std::get<std::string>(token.value);
      values[name] = std::make_tuple<SymbolType, SymbolDebugInformation,
                                     std::variant<std::string, Regexp *>>(
          SymbolType::Const,
          {SymbolType::Const, token.lineno, token.offset, token.length, name,
           s},
          s);
      statestack.pop(); // const
      statestack.pop(); // name
      break;
    }
    case ParserState::Normal: {
      if (token.type != TOK_LITSTRING && token.type != TOK_NAME) {
        failing = true;
        std::printf("expected either a string literal or a bare listing of "
                    "characters. invalid token %s - %s\n",
                    reverse_token_type[token.type],
                    std::get<std::string>(token.value).c_str());
        break;
      }
      std::string ns = std::get<std::string>(persist);
      std::string replaced = std::get<std::string>(token.value);
      for (auto cp : codepoints(replaced))
        if (!gen_lexer_normalisations.count(cp)) {
          std::printf("registered normalisation '%s' for '%s'\n", ns.c_str(),
                      cp.c_str());
          gen_lexer_normalisations[cp] = ns;
        } else if (gen_lexer_normalisations[cp] != cp)
          std::printf(
              "a normalisation of '%s' has already been registered for '%s'\n",
              gen_lexer_normalisations[cp].c_str(), cp.c_str());
      break;
    }
    }
  } while (!failing);
}

std::vector<Regexp *> get_all_finals(Regexp *exp) {
  std::vector<Regexp *> ends;
  switch (exp->type) {
  case RegexpType::Symbol:
    std::printf("[ICE] unresolved symbol left in tree\n");
    unreachable();
    break;
  case RegexpType::Alternative:
    for (auto p : exp->children)
      for (auto q : get_all_finals(p))
        ends.push_back(q);
    break;
  case RegexpType::Concat:
    for (auto q : get_all_finals(exp->children.back()))
      ends.push_back(q);
    break;
  case RegexpType::Nested:
    for (auto q : get_all_finals(std::get<Regexp *>(exp->inner)))
      ends.push_back(q);
    break;
  case RegexpType::Dot:
    ends.push_back(exp);
    break;
  case RegexpType::Literal:
    ends.push_back(exp);
    break;

  case RegexpType::CharacterClass: {
    ends.push_back(exp);
    break;
  }
  case RegexpType::Assertion:
    ends.push_back(exp);
    break;
  case RegexpType::Escape: {
    std::printf("Escapes not yet implemented\n");
    unreachable();
    break;
  }
  }
  return ends;
}

NFANode<std::string> *NParser::compile() {
  parse();
#ifdef TEST
  std::printf("== all defined rules ==\n");
  for (auto &it : values) {
    if (std::get<0>(it.second) == SymbolType::Define) {
      auto rule = std::get<Regexp *>(std::get<2>(it.second));
      std::printf("rule \"%s\" - <%s>\n", it.first.c_str(),
                  rule->to_str().c_str());
    }
  }
#endif
  NFANode<std::string> *root_node = new NFANode<std::string>{"root"};
  root_node->start = true;
  std::multimap<const Regexp *, NFANode<std::string> *> node_cache;
  std::set<NFANode<std::string> *> toplevels;

  for (auto &it : find_rules()) {
    auto rule = std::get<Regexp *>(std::get<2>(values[it]));
    bool leading = true;
    auto node = rule->compile(node_cache, root_node, "", leading);
    toplevels.insert(node); // nullptr marks toplevel rule
  }

  // reconstruct_forwards(root_node, {});

  // mark final states and give them names
  for (auto &it : values) {
    if (std::get<0>(it.second) == SymbolType::Define) {
      auto rule = std::get<Regexp *>(std::get<2>(it.second));
      auto ends = get_all_finals(rule);
      for (auto end : ends) {
        if (node_cache.count(end)) {
          for (auto iter = node_cache.find(end); iter != node_cache.end();
               ++iter) {
            auto node = deep_output_end(iter->second);
            if (toplevels.count(iter->second))
              goto yep;
            continue; // no need for this to be final
          yep:;
            std::printf(
                "Regexp %p (node %p) has been marked as final: [%s] [%s]\n",
                end, node, end->mangle().c_str(),
                node->named_rule
                    .value_or(node->state_info.value_or("<unknown>"))
                    .c_str());
            node->final = true;
            node->named_rule = it.first;
          }
        } else {
          std::printf("Regexp %p has no corresponding node\n", end);
        }
      }
    }
  }
  root_node->max_opt_steps = max_opt_steps;
  root_node->optimise({});
  return root_node;
}

std::set<std::string> NParser::find_leaf_rules() const {
  std::set<std::string> leafs;
  for (auto &it : values) {
    if (std::get<0>(it.second) == SymbolType::Define) {
      auto rule = std::get<Regexp *>(std::get<2>(it.second));
      if (rule->is_leaf)
        leafs.insert(it.first);
    }
  }
  return leafs;
}

std::set<std::string> NParser::find_rules() const {
  std::set<std::string> nodes;
  for (auto &it : values) {
    if (std::get<0>(it.second) == SymbolType::Define) {
      nodes.insert(it.first);
    }
  }
  return nodes;
}

template <typename T> void NFANode<T>::print() {
  printf("(%s\"%s\" ", named_rule.has_value() ? "final " : "",
         state_info->c_str());
  for (auto t : get_outgoing_transitions(/* inner = */ true))
    std::printf("\"-%s->\" ",
                std::holds_alternative<EpsilonTransitionT>(t->input)
                    ? "<e>"
                    : std::holds_alternative<AnythingTransitionT>(t->input)
                          ? "<.>"
                          : std::string{std::get<char>(t->input)}.c_str()),
        t->target->print();
  std::printf(")");
}

template <typename T> void NFANode<T>::print_dot() {
  std::set<NFANode<T> *, NFANodePointerComparer<T>> nodes;
  std::set<NFANode<T> *> anodes;
  std::unordered_set<CanonicalTransition<
      NFANode<T>, std::variant<char, EpsilonTransitionT, AnythingTransitionT>>>
      transitions;
  aggregate_dot(nodes, anodes, transitions);
  std::printf("%s\n", gen_dot(anodes, transitions).c_str());
}

template <typename T>
std::string get_name(std::set<NFANode<T> *> nodes, bool simple = false,
                     bool ptr = false) {
  std::ostringstream oss;
  if (!ptr)
    oss << "{ ";
  for (auto node : nodes) {
    if (!ptr)
      oss << node->named_rule.value_or(node->state_info.value_or("<unknown>"));
    if (!simple && !ptr)
      oss << string_format("(%p)", node);
    if (ptr)
      oss << string_format("%p", node);

    if (!ptr)
      oss << " ";
  }

  if (!ptr)
    oss << "}";
  return oss.str();
}

template <typename T>
std::string get_name(NFANode<T> *node, bool simple = false, bool ptr = false) {
  std::ostringstream oss;
  if (!ptr)
    oss << "{ ";
  if (!ptr)
    oss << node->named_rule.value_or(node->state_info.value_or("<unknown>"));
  if (!simple && !ptr)
    oss << string_format("(%p)", node);
  if (ptr)
    oss << string_format("%p", node);

  if (!ptr)
    oss << " }";
  return oss.str();
}

std::string sanitised(char c) {
  if (c < 10)
    return "<" + std::to_string((int)c) + ">";
  else
    return std::string{c};
}

template <typename T>
std::string NFANode<T>::gen_dot(
    std::set<NFANode<T> *> nodes,
    std::unordered_set<
        CanonicalTransition<NFANode<T>, std::variant<char, EpsilonTransitionT,
                                                     AnythingTransitionT>>>
        transitions) {
  std::ostringstream oss;
  constexpr auto ss_end = "\n\t";
  oss << "digraph finite_state_machine {" << ss_end;
  oss << "rankdir=LR;" << ss_end;
  oss << "size=\"8,5\";" << ss_end;
  int node_id = 0, error = 1000000;
  std::map<NFANode<T> *, int> nodeids;
  for (auto node : nodes) {
    nodeids[node] = node_id++;
    oss << "node [shape = "
        << (node->start ? "square" : node->final ? "doublecircle" : "circle")
        << (", label = \"" + get_name(node, true) + "\\nat " +
            string_format("%p", node) + "\"")
        << "] LR_" << nodeids[node] << ";" << ss_end;
  }
  for (auto tr : transitions) {
    int fid, tid;
    if (nodeids.count(tr.source) < 1) {
      printf("Cannot find any node corresponding to %p\n", tr.source);
      fid = error++;
    } else
      fid = nodeids[tr.source];

    if (nodeids.count(tr.target) < 1) {
      printf("Cannot find any node corresponding to %p\n", tr.target);
      tid = error++;
    } else
      tid = nodeids[tr.target];

    oss << "LR_" << fid << " -> LR_" << tid << " [ label = \""
        << (std::holds_alternative<EpsilonTransitionT>(tr.input)
                ? "<Epsilon>"
                : std::holds_alternative<AnythingTransitionT>(tr.input)
                      ? (std::string{"<"} +
                         (std::get<AnythingTransitionT>(tr.input).inverted
                              ? "None"
                              : "Any") +
                         " of '" +
                         (std::get<AnythingTransitionT>(tr.input).values) +
                         "'>")
                            .c_str()
                      : sanitised(std::get<char>(tr.input)).c_str())
        << " -> " << get_name(tr.target, true) << "\\n@"
        << string_format("%p", tr.target) << "\" ];" << ss_end;
  }
  oss << "}";
  return oss.str();
}

template <typename T>
void NFANode<T>::aggregate_dot(
    std::set<NFANode<T> *, NFANodePointerComparer<T>> &nodes,
    std::set<NFANode<T> *> &anodes,
    std::unordered_set<
        CanonicalTransition<NFANode<T>, std::variant<char, EpsilonTransitionT,
                                                     AnythingTransitionT>>>
        &transitions) {
  if (anodes.count(this))
    return;
  auto pos = nodes.find(this);
  if (pos == nodes.end() || *pos != this) {
    // not yet explored
    nodes.insert(this);
    anodes.insert(this);
    for (auto transition : get_outgoing_transitions(/* inner = */ true)) {
      transitions.insert({this, transition->target, transition->input});
      transition->target->aggregate_dot(nodes, anodes, transitions);
    }
    auto default_transition = deep_output_end(this)->default_transition;
    if (default_transition) {
      transitions.insert({this, default_transition, '\0'});
      default_transition->aggregate_dot(nodes, anodes, transitions);
    }
    // for (auto transition : incoming_transitions) {
    //   transitions.insert({this, transition->target, '<'});
    // }
  }
}

template <typename T> void DFANode<T>::print_dot() {
  std::set<DFANode<T> *, DFANodePointerComparer<T>> nodes;
  std::set<DFANode<T> *> anodes;
  std::unordered_set<CanonicalTransition<DFANode<T>, char>> transitions;
  aggregate_dot(nodes, anodes, transitions);
  std::printf("%s\n", gen_dot(anodes, transitions).c_str());
}

auto print_asserts(std::vector<RegexpAssertion> asserts) {
  std::ostringstream oss;
  for (auto a : asserts)
    switch (a) {
    case RegexpAssertion::SetPosition:
      oss << " SetPosition";
      break;
    case RegexpAssertion::LineBeginning:
      oss << " LineBeginning";
      break;
    case RegexpAssertion::LineEnd:
      oss << " LineEnd";
      break;
    case RegexpAssertion::TrueBeginning:
      oss << " TrueBeginning";
      break;
    case RegexpAssertion::MatchBeginning:
      oss << " MatchBeginning";
      break;
    }
  return oss.str().substr(1);
}

template <typename T>
std::string DFANode<T>::gen_dot(
    std::set<DFANode<T> *> nodes,
    std::unordered_set<CanonicalTransition<DFANode<T>, char>> transitions) {
  std::ostringstream oss;
  constexpr auto ss_end = "\n\t";
  oss << "digraph finite_state_machine {" << ss_end;
  oss << "rankdir=LR;" << ss_end;
  oss << "size=\"8,5\";" << ss_end;
  int node_id = 0, error = 1000000;
  std::map<DFANode<T> *, int> nodeids;
  for (auto node : nodes) {
    nodeids[node] = node_id++;
    oss << "node [shape = "
        << (node->start ? "square" : node->final ? "doublecircle" : "circle")
        << (", label = \"" + get_name(node->state_info.value(), false, true) +
            "\\nat " + string_format("%p", node))
        << (node->assertions.size() > 0
                ? string_format("[asserts %s]",
                                print_asserts(node->assertions).c_str())
                : "")
        << '"' << "] LR_" << nodeids[node] << ";" << ss_end;
  }
  for (auto tr : transitions) {
    int fid, tid;
    if (nodeids.count(tr.source) < 1) {
      printf("Cannot find any node corresponding to %p\n", tr.source);
      fid = error++;
    } else
      fid = nodeids[tr.source];

    if (nodeids.count(tr.target) < 1) {
      printf("Cannot find any node corresponding to %p\n", tr.target);
      tid = error++;
    } else
      tid = nodeids[tr.target];

    oss << "LR_" << fid << " -> LR_" << tid << " [ label = \""
        << sanitised(tr.input) << " -> "
        << get_name(tr.target->state_info.value(), false, true) << "\\n@"
        << string_format("%p", tr.target) << "\" ];" << ss_end;
  }
  oss << "}";
  return oss.str();
}

template <typename T>
void DFANode<T>::aggregate_dot(
    std::set<DFANode<T> *, DFANodePointerComparer<T>> &nodes,
    std::set<DFANode<T> *> &anodes,
    std::unordered_set<CanonicalTransition<DFANode<T>, char>> &transitions) {
  if (anodes.count(this))
    return;
  auto pos = nodes.find(this);
  if (pos == nodes.end() || *pos != this) {
    // not yet explored
    nodes.insert(this);
    anodes.insert(this);
    for (auto transition : outgoing_transitions) {
      transitions.insert({this, transition->target, transition->input});
      transition->target->aggregate_dot(nodes, anodes, transitions);
    }
    if (default_transition) {
      transitions.insert({this, default_transition, '\0'});
      default_transition->aggregate_dot(nodes, anodes, transitions);
    }
    // for (auto transition : incoming_transitions) {
    //   transitions.insert({this, transition->target, '<'});
    // }
  }
}

template <typename T>
static std::set<NFANode<T> *>
get_states(const NFANode<T> *state,
           const std::variant<char, EpsilonTransitionT> trv) {
  std::set<NFANode<T> *> states;
  std::variant<char, EpsilonTransitionT, AnythingTransitionT> transition;
  if (std::holds_alternative<char>(trv))
    transition = std::get<char>(trv);
  else
    transition = EpsilonTransition;
  for (auto tr : state->outgoing_transitions) {
    if (tr->input == transition)
      states.insert(tr->target);
  }
  return states;
}

template <typename T>
static std::set<NFANode<T> *>
get_states(const std::set<NFANode<T> *> &states,
           const std::variant<char, EpsilonTransitionT> transition) {
  std::set<NFANode<T> *> cstates;
  for (auto state : states) {
    auto ss = get_states(state, transition);
    cstates.insert(ss.begin(), ss.end());
  }
  return cstates;
}

template <typename T>
static std::set<NFANode<T> *> get_epsilon_closure(
    NFANode<T> *node,
    std::map<std::set<NFANode<T> *>, std::set<NFANode<T> *>> epsilon_closures) {
  std::set<NFANode<T> *> key;
  key.insert(node);

  if (epsilon_closures.count(key))
    return epsilon_closures[key];

  std::set<NFANode<T> *> eps_states;

  std::queue<NFANode<T> *> remaining;
  remaining.push(node);

  while (!remaining.empty()) {
    auto current = remaining.front();
    eps_states.insert(current);
    for (auto ep : get_states(current, EpsilonTransition)) {
      if (!eps_states.count(ep))
        remaining.push(ep);
    }
    remaining.pop();
  }
  epsilon_closures[key] = eps_states;
  return eps_states;
}

template <typename T>
static std::set<NFANode<T> *> get_epsilon_closure(
    const std::set<NFANode<T> *> nodes,
    std::map<std::set<NFANode<T> *>, std::set<NFANode<T> *>> epsilon_closures) {
  if (epsilon_closures.count(nodes))
    return epsilon_closures[nodes];

  std::set<NFANode<T> *> eps_states;

  for (auto node : nodes) {
    auto ss = get_epsilon_closure(node, epsilon_closures);
    eps_states.insert(ss.begin(), ss.end());
  }
  epsilon_closures[nodes] = eps_states;
  return eps_states;
}

template <typename T> std::set<char> all_alphabet(NFANode<T> *node) {
  std::set<char> ss;
  for (auto tr : node->outgoing_transitions) {
    if (std::holds_alternative<EpsilonTransitionT>(tr->input))
      continue;
    if (std::holds_alternative<AnythingTransitionT>(tr->input))
      std::printf("[ICE] All compound transitions haven't been resolved\n");
    else
      ss.insert(std::get<char>(tr->input));
  }
  return ss;
}

template <typename T>
std::set<char> all_alphabet(std::set<NFANode<T> *> nodes) {
  std::set<char> ss;
  for (auto node : nodes) {
    auto al = all_alphabet(node);
    ss.insert(al.begin(), al.end());
  }
  return ss;
}

template <typename T>
void DFANode<T>::add_transition(Transition<DFANode<T>, char> *tr) {
  for (auto vtr : outgoing_transitions) {
    if (vtr->input == tr->input) {
      std::printf("transition to two states with one input requested\n");
      return;
    }
  }
  Transition<DFANode<T>, char> tbr{this, tr->input};
  tr->target->incoming_transitions.insert(new decltype(tbr){tbr});
  outgoing_transitions.insert(tr);
}

template <typename T> DFANode<std::set<NFANode<T> *>> *NFANode<T>::to_dfa() {
  DFANode<std::set<NFANode<T> *>> *dfa_root = nullptr;
  std::map<std::string, DFANode<std::set<NFANode<T> *>> *> dfa_map;

  std::set<NFANode<T> *> init = get_epsilon_closure(this, {});

  std::queue<std::set<NFANode<T> *>> remaining;
  remaining.push(init);

  std::set<std::set<NFANode<T> *>> visited;

  while (!remaining.empty()) {
    std::set<NFANode<T> *> current = remaining.front();
    remaining.pop();

    if (visited.count(current))
      continue;
    std::vector<RegexpAssertion> assertions;

    for (auto c : current) {
      auto as = c->assertions;
      for (auto a : as)
        assertions.push_back(a);
    }

    slts.show("[{<red>}DFAGen{<clean>}] Generating for node set "
              "{<green>}'%s'{<clean>}",
              get_name(current).c_str());

    visited.insert(current);
    DFANode<std::set<NFANode<T> *>> *dfanode;

    auto currentname = get_name(current);

    if (dfa_map.count(currentname))
      dfanode = dfa_map[currentname];
    else
      dfanode = dfa_map[currentname] =
          new DFANode<std::set<NFANode<T> *>>{current};
    for (auto s : current) {
      if (s->final) {
        // std::printf("state %s was final, so marking %s as such\n",
        //             get_name(s).c_str(), get_name(current).c_str());
        dfanode->final = true;
      }
      if (s->start) {
        // std::printf("state %s was start, so marking %s as such\n",
        //             get_name(s).c_str(), get_name(current).c_str());
        dfanode->start = true;
      }
    }

    if (dfa_root == nullptr)
      dfa_root = dfanode;

    auto salphabets = all_alphabet(current);
    std::vector<char> alphabets{salphabets.begin(), salphabets.end()};
    std::mutex m;
    std::for_each(
        std::execution::par_unseq, alphabets.begin(), alphabets.end(),
        [&](auto qq) {
          slts.show("[{<red>}DFAGen{<clean>}] [{<red>}Resolution{<clean>}] "
                    "{<green>}'%s'{<clean>} :: {<magenta>}%d{<clean>} = ('%c')",
                    get_name(current).c_str(), qq, qq);

          std::set<NFANode<T> *> eps_state;
          DFANode<std::set<NFANode<T> *>> *nextdfanode;
          {
            std::lock_guard lg{m};
            eps_state = get_epsilon_closure(get_states(current, qq), {});
          }
          auto epsname = get_name(eps_state);

          if (dfa_map.count(epsname))
            nextdfanode = dfa_map[epsname];
          else {
            nextdfanode = dfa_map[epsname] =
                new DFANode<std::set<NFANode<T> *>>{eps_state};
          }
          dfanode->add_transition(
              new Transition<DFANode<std::set<NFANode<T> *>>, char>{nextdfanode,
                                                                    qq});
          if (!visited.count(eps_state))
            remaining.push(eps_state);
        });
    dfanode->assertions = assertions;

    NFANode<T> *defl = nullptr;
    for (auto s : current)
      if (s->default_transition) {
        defl = s->default_transition;
        break;
      }
    if (defl) {
      slts.show("[{<red>}DFAGen{<clean>}] [{<red>}Default{<clean>}] "
                "{<green>}'%s'{<clean>} :: transition %p\n",
                get_name(current).c_str(), defl);

      std::set<NFANode<T> *> eps_state = get_epsilon_closure(defl, {});
      DFANode<std::set<NFANode<T> *>> *nextdfanode;

      auto epsname = get_name(eps_state);

      if (dfa_map.count(epsname))
        nextdfanode = dfa_map[epsname];
      else {
        nextdfanode = dfa_map[epsname] =
            new DFANode<std::set<NFANode<T> *>>{eps_state};
      }

      dfanode->default_transition_to(nextdfanode);

      if (!visited.count(eps_state))
        remaining.push(eps_state);
    }
  }

  return dfa_root;
}

template <typename T>
void DFACCodeGenerator<T>::generate(
    DFANode<std::set<NFANode<T> *>> *node,
    std::set<DFANode<std::set<NFANode<T> *>> *> visited) {
  if (visited.count(node))
    return;
  visited.insert(node);
  if (node->start)
    node->print_dot();

  // generate any choice and add to output_cases
  std::ostringstream output_case;
  for (auto tr : node->outgoing_transitions) {
    output_case << "case " << (int)tr->input << ":";
    auto node = tr->target;
    if (node->final) {
      // emit tags
      auto em = false;
      std::set<std::string> emitted;
      for (auto state : node->state_info.value()) {
        if (state->named_rule.has_value()) {
          auto val = state->named_rule.value();
          if (emitted.count(val))
            continue;
          emitted.insert(val);
          em = true;
          output_case << string_format("__nlex_emit(\"%s\");", val.c_str());
        }
      }

      if (!em) {
        output_case << string_format("__nlex_emit(\"%s\");",
                                     "<Unknown final state>");
      }
    }
    output_case << string_format(
        "__%s();",
        get_name(tr->target->state_info.value(), false, true).c_str());
    output_case << "break;";
    generate(tr->target, visited);
  }
  output_cases.push_front(std::make_pair(
      get_name(node->state_info.value(), false, true), output_case.str()));
  if (node->start)
    output_cases.push_front(std::make_pair(
        "root", get_name(node->state_info.value(), false, true)));
}

template <typename T> std::string DFACCodeGenerator<T>::output() {
  std::ostringstream oss;
  auto root = output_cases.front();
  output_cases.pop_front();
  std::set<std::string> visit;
  for (auto _case : output_cases) {
    visit.insert(_case.first);
    oss << "inline static void " << string_format("__%s()", _case.first.c_str())
        << ";";
  }
  // generate root function
  oss << "void __nlex_root() {" << string_format("__%s();", root.second.c_str())
      << "}";
  for (auto _case : output_cases) {
    if (!visit.count(_case.first))
      continue;
    oss << "inline static void " << string_format("__%s()", _case.first.c_str())
        << "{";
    oss << "__nlex_advance();";
    if (_case.second.size() > 0) {
      oss << "switch(__nlex_current()) {";
      oss << _case.second;
      oss << "}";
    }
    oss << "}";
  }
  return oss.str();
}

template <typename T>
void DFANLVMCodeGenerator<T>::generate(
    DFANode<std::set<NFANode<T> *>> *node,
    std::set<DFANode<std::set<NFANode<T> *>> *> visited) {
  std::map<DFANode<std::set<NFANode<T> *>> *, llvm::BasicBlock *> blk{};
  generate(node, visited, blk);
}

using namespace llvm;
template <typename T>
void DFANLVMCodeGenerator<T>::generate(
    DFANode<std::set<NFANode<T> *>> *node,
    std::set<DFANode<std::set<NFANode<T> *>> *> visited,
    std::map<DFANode<std::set<NFANode<T> *>> *, BasicBlock *> &blocks) {
  if (visited.count(node))
    return;
  visited.insert(node);
  slts.show("[{<red>}NodeGen{<clean>}] Generating for node set "
            "{<green>}'%s'{<clean>}",
            get_name(node->state_info.value()).c_str());
  // generate any choice and add to output_cases
  BasicBlock *BB = BasicBlock::Create(builder.module.TheContext,
                                      get_name(node->state_info.value()),
                                      builder.module.main());
  llvm::BasicBlock *BBnode = BB;
  BasicBlock *BBend = BasicBlock::Create(
      builder.module.TheContext, get_name(node->state_info.value()) + "{::}E",
      builder.module.main());
  builder.module.Builder.SetInsertPoint(BBend);
  // restore string position and return with tag

  if (!node->final) {
    builder.module.Builder.CreateCall(
        builder.module.nlex_restore,
        {builder.module.Builder.CreateLoad(
            builder.module.last_final_state_position)});
  } else {
    builder.module.Builder.CreateCall(
        builder.module.nlex_restore,
        {builder.module.Builder.CreateInBoundsGEP(
            builder.module.Builder.CreateCall(builder.module.nlex_current_p),
            {llvm::ConstantInt::get(
                llvm::Type::getInt32Ty(builder.module.TheContext), -1)})});
  }
  builder.module.Builder.CreateBr(builder.module.BBfinalise);

  builder.module.Builder.SetInsertPoint(BB);
  // if there are assertions, apply them now
  auto tstart = builder.module.Builder.CreateCall(builder.module.nlex_start);
  auto tnext = builder.module.Builder.CreateCall(builder.module.nlex_current_p);
  auto tprev = builder.module.Builder.CreateGEP(
      tnext, {llvm::ConstantInt::get(
                 llvm::Type::getInt32Ty(builder.module.TheContext), -1)});
  auto assert = false;
  for (auto assertion : node->assertions) {
    switch (assertion) {
    case RegexpAssertion::SetPosition: {
      slts.show("[{<red>}ERR{<clean>}] Unimplemented assertion "
                "{<magenta>}\\K{<clean>}");
      abort();
      break;
    }
    case RegexpAssertion::LineBeginning: {
      BBnode = BasicBlock::Create(builder.module.TheContext,
                                  "assertPass(^)::" +
                                      get_name(node->state_info.value()),
                                  builder.module.main());
      // check if we're at the beginning of the string (fed_string==true_start)
      // or last character (fed_string-1) is \n
      builder.module.Builder.CreateCondBr(
          builder.module.Builder.CreateOr(
              builder.module.Builder.CreateICmpEQ(tprev, tstart),
              builder.module.Builder.CreateICmpEQ(
                  builder.module.Builder.CreateLoad(tprev),
                  llvm::ConstantInt::get(
                      llvm::Type::getInt8Ty(builder.module.TheContext),
                      (int)'\n')))
          /* assert pass */
          ,
          BBnode
          /* assert fail */
          ,
          BBend);
      builder.module.Builder.SetInsertPoint(BBnode);
      // BB = BBnode;
      break;
    }
    case RegexpAssertion::LineEnd: {
      BBnode = BasicBlock::Create(builder.module.TheContext,
                                  "assertPass($)::" +
                                      get_name(node->state_info.value()),
                                  builder.module.main());
      // check if we're at the end of the string (next char is zero or \n)
      auto nextc = builder.module.Builder.CreateLoad(tnext);
      builder.module.Builder.CreateCondBr(
          builder.module.Builder.CreateOr(
              builder.module.Builder.CreateICmpEQ(
                  nextc,
                  llvm::ConstantInt::get(
                      llvm::Type::getInt8Ty(builder.module.TheContext), 0)),
              builder.module.Builder.CreateICmpEQ(
                  nextc, llvm::ConstantInt::get(
                             llvm::Type::getInt8Ty(builder.module.TheContext),
                             (int)'\n')))
          /* assert pass */
          ,
          BBnode
          /* assert fail */
          ,
          BBend);
      builder.module.Builder.SetInsertPoint(BBnode);
      // BB = BBnode;
      break;
    }
    case RegexpAssertion::TrueBeginning: {
      BBnode = BasicBlock::Create(builder.module.TheContext,
                                  "assertPass(A)::" +
                                      get_name(node->state_info.value()),
                                  builder.module.main());
      // check if we're at the beginning of the string (fed_string==true_start)
      builder.module.Builder.CreateCondBr(
          builder.module.Builder.CreateICmpEQ(tprev, tstart)
          /* assert pass */
          ,
          BBnode
          /* assert fail */
          ,
          BBend);
      builder.module.Builder.SetInsertPoint(BBnode);
      // BB = BBnode;
      break;
    }
    case RegexpAssertion::MatchBeginning: {
      slts.show("[{<red>}ERR{<clean>}] Unimplemented assertion "
                "{<magenta>}\\G{<clean>}");
      abort();
      break;
    }
    }
  }
  if (node->final) {
    // store the tag and string position upon getting here
    auto em = false;
    std::set<std::string> emitted;
    std::string emit;
    for (auto state : node->state_info.value()) {
      if (state->named_rule.has_value()) {
        auto val = state->named_rule.value();
        if (emitted.count(val))
          continue;
        emitted.insert(val);
        emit = val;
        em = true;
      }
    }
    builder.module.Builder.CreateStore(
        builder.get_or_create_tag(em ? emit : "<Unknown State>"),
        builder.module.last_tag);
    builder.module.Builder.CreateStore(
        builder.module.Builder.CreateCall(builder.module.nlex_current_p, {}),
        builder.module.last_final_state_position);
    builder.module.Builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt8Ty(builder.module.TheContext),
                               (int)!em),
        builder.module.nlex_errc);
  }
  builder.module.Builder.CreateCall(builder.module.nlex_next, {});
  auto readv = builder.module.Builder.CreateCall(builder.module.nlex_current_f,
                                                 {}, "readv");

  blocks[node] = BB;
  std::set<char> outgoings;
  for (auto tr : node->outgoing_transitions) {
    outgoings.insert(tr->input);
  }
  llvm::BasicBlock *deflBB = nullptr;
  if (node->default_transition != nullptr) {
    if (!blocks.count(node->default_transition))
      generate(node->default_transition, visited, blocks);
    deflBB = blocks[node->default_transition];
    auto bb_ = BasicBlock::Create(builder.module.TheContext, "",
                                  builder.module.main());
    builder.module.Builder.SetInsertPoint(bb_);
    // consume as many chars as needed for a complete unicode codepoint
    {
      auto bb0 = BasicBlock::Create(builder.module.TheContext, "",
                                    builder.module.main());
      auto bb1 = BasicBlock::Create(builder.module.TheContext, "",
                                    builder.module.main());
      auto bb2 = BasicBlock::Create(builder.module.TheContext, "",
                                    builder.module.main());
      decltype(bb0) bbs[] = {bb0, bb1, bb2};
      auto bbsel1 = BasicBlock::Create(builder.module.TheContext, "",
                                       builder.module.main());
      auto bbsel2 = BasicBlock::Create(builder.module.TheContext, "",
                                       builder.module.main());
      auto bbphi = BasicBlock::Create(builder.module.TheContext, "",
                                      builder.module.main());
      auto valskip = builder.module.Builder.CreateCall(
          builder.module.nlex_get_utf8_length, {readv});
      auto lt2 = builder.module.Builder.CreateICmpSLT(
          valskip, llvm::ConstantInt::get(
                       llvm::Type::getInt32Ty(builder.module.TheContext), 2));
      builder.module.Builder.CreateCondBr(lt2, bbsel1, bbsel2);
      builder.module.Builder.SetInsertPoint(bbsel1);
      auto s1 = builder.module.Builder.CreateSelect(
          builder.module.Builder.CreateICmpEQ(
              valskip,
              llvm::ConstantInt::get(
                  llvm::Type::getInt32Ty(builder.module.TheContext), 0)),
          llvm::BlockAddress::get(builder.module.main(), deflBB),
          llvm::BlockAddress::get(builder.module.main(), bb2));
      builder.module.Builder.CreateBr(bbphi);
      builder.module.Builder.SetInsertPoint(bbsel2);
      auto s2 = builder.module.Builder.CreateSelect(
          builder.module.Builder.CreateICmpEQ(
              valskip,
              llvm::ConstantInt::get(
                  llvm::Type::getInt32Ty(builder.module.TheContext), 2)),
          llvm::BlockAddress::get(builder.module.main(), bb1),
          llvm::BlockAddress::get(builder.module.main(), bb0));
      builder.module.Builder.CreateBr(bbphi);
      builder.module.Builder.SetInsertPoint(bbphi);
      auto phi = builder.module.Builder.CreatePHI(
          llvm::PointerType::get(
              llvm::Type::getInt8Ty(builder.module.TheContext), 0),
          2);
      phi->addIncoming(s1, bbsel1);
      phi->addIncoming(s2, bbsel2);

      for (auto i = 0; i < 3; i++) {
        auto bb = bbs[i];
        builder.module.Builder.SetInsertPoint(bb);
        builder.module.Builder.CreateCall(builder.module.nlex_next);
        auto val =
            builder.module.Builder.CreateCall(builder.module.nlex_current_f);
        builder.module.add_value_to_token(val);
        if (i < 2)
          builder.module.Builder.CreateBr(bbs[i + 1]);
        else
          builder.module.Builder.CreateBr(deflBB);
      }
      builder.module.Builder.SetInsertPoint(bbphi);
      auto ibr = builder.module.Builder.CreateIndirectBr(phi, 4);
      ibr->addDestination(deflBB); // 0
      ibr->addDestination(bb2);    // 1
      ibr->addDestination(bb1);    // 2
      ibr->addDestination(bb0);    // 3
    }
    deflBB = bb_;
  }
  builder.module.Builder.SetInsertPoint(BBnode);
  if (outgoings.size() > 0) {
    if (deflBB)
      builder.module.add_value_to_token(readv);
    auto switchinst = builder.module.Builder.CreateSwitch(
        readv, deflBB ? deflBB : BBend, outgoings.size());
    if (deflBB) // don't go past the end
      switchinst->addCase(
          ConstantInt::get(IntegerType::get(builder.module.TheContext, 8), "0",
                           10),
          BBend);
    for (auto tr : node->outgoing_transitions) {
      // todo: be less naive
      if (!blocks.count(tr->target))
        generate(tr->target, visited, blocks);

      auto jdst = blocks[tr->target];
      auto dst = BasicBlock::Create(builder.module.TheContext, "casejmp",
                                    builder.module.main());
      builder.module.Builder.SetInsertPoint(dst);
      if (!deflBB)
        builder.module.add_char_to_token(tr->input);
      builder.module.Builder.CreateBr(jdst);
      builder.module.Builder.SetInsertPoint(BBnode);
      switchinst->addCase(
          ConstantInt::get(IntegerType::get(builder.module.TheContext, 8),
                           std::to_string((int)tr->input), 10),
          dst);
    }
  } else {
    if (deflBB) {
      builder.module.add_value_to_token(readv);
      builder.module.Builder.CreateCondBr(
          builder.module.Builder.CreateICmpNE(
              readv,
              ConstantInt::get(Type::getInt8Ty(builder.module.TheContext), 0)),
          deflBB, BBend);
    } else
      builder.module.Builder.CreateBr(BBend);
  }
  if (node->start)
    root_bb = BB;
}

template <typename T> std::string DFANLVMCodeGenerator<T>::output() {
  builder.first_root = this->root_bb;
  builder.end();
  return "";
}

template <typename T> std::string CodeGenerator<T>::output() { __asm("int3"); }

template <typename T>
void CodeGenerator<T>::generate(
    DFANode<std::set<NFANode<T> *>> *node,
    std::set<DFANode<std::set<NFANode<T> *>> *> visited) {
  __asm("int3");
}

template <typename T> void CodeGenerator<T>::run(NFANode<T> *node) {
  if (properties.start_phase == CodegenStartPhase::RegexpPhase) {
    std::printf("call the proper run function, noob\n");
    return;
  }
  // if (properties.start_phase == CodegenStartPhase::NFAPhase)
  // generate(node);
  // else
  generate(node->to_dfa());
}

std::string exec(const char *cmd, const bool &run) {
  std::array<char, 256> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    return "";
  }
  while (run && fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

#ifdef TEST
int main() {
  NParser parser;
  parser.lexer = std::make_unique<NLexer>("");
  NFANode<std::string> *root;
  DFACCodeGenerator<std::string> cg;
  DFANLVMCodeGenerator<std::string> nlvmg;
  nlvmg.builder.begin();
  while (1) {
    std::string line;
    std::cout << "> ";
    std::getline(std::cin, line);
    if (line == ".tree") {
      root = parser.compile();
      root->print();
      continue;
    } else if (line == ".dot") {
      root = parser.compile();
      root->print_dot();
      continue;
    } else if (line == ".end") {
      // root = parser.compile();
      // root->print();
      break;
    } else if (line == ".opt=") {
      std::cout << "opt=? ";
      std::cin >> parser.max_opt_steps;
      continue;
    } else if (line == ".cg") {
      root = parser.compile();
      cg.run(root);
      std::cout << cg.output();
      continue;
    } else if (line == ".gengraph") {
      parser.generate_graph = !parser.generate_graph;
      printf("will %sgenerate a graph\n", parser.generate_graph ? "" : "not ");
      continue;
    } else if (line == ".nlvm") {
      root = parser.compile();
      auto rootdfa = root->to_dfa();
      if (parser.generate_graph) {
        std::set<DFANode<std::set<NFANode<std::string> *>> *,
                 DFANodePointerComparer<std::set<NFANode<std::string> *>>>
            nodes;
        std::set<DFANode<std::set<NFANode<std::string> *>> *> anodes;
        std::unordered_set<CanonicalTransition<
            DFANode<std::set<NFANode<std::string> *>>, char>>
            transitions;

        rootdfa->aggregate_dot(nodes, anodes, transitions);

        std::string name = std::tmpnam(nullptr);
        auto fp = std::fopen(name.c_str(), "w+");
        std::fprintf(fp, "%s\n", rootdfa->gen_dot(anodes, transitions).c_str());
        std::fclose(fp);

        bool run = true;

        std::thread render{[&]() {
          nlvmg.builder.prepare(parser.gen_lexer_normalisations,
                                parser.gen_lexer_stopwords,
                                parser.gen_lexer_ignores);

          nlvmg.generate(rootdfa);
          nlvmg.output();
          run = false;
        }};
        exec(("../tools/wm '" + name + "'").c_str(), run);
        render.join();
      } else {
        nlvmg.builder.prepare(parser.gen_lexer_normalisations,
                              parser.gen_lexer_stopwords,
                              parser.gen_lexer_ignores);

        nlvmg.generate(rootdfa);
        nlvmg.output();
      }
      continue;
    }
    parser.repl_feed(line);
    parser.parse();
  }
}
#endif
