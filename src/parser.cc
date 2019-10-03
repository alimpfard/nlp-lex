#define TEST

#include "parser.hpp"
#include "lexer.hpp"
#include "optimise.tcc"

#include <iostream>
#include <memory>
#include <sstream>
#include <stdarg.h>

std::string string_format(const std::string fmt_str, ...) {
  int final_n,
      n = ((int)fmt_str.size()) *
          2; /* Reserve two times as much as the length of the fmt_str */
  std::unique_ptr<char[]> formatted;
  va_list ap;
  while (1) {
    formatted.reset(
        new char[n]); /* Wrap the plain char array into the unique_ptr */
    strcpy(&formatted[0], fmt_str.c_str());
    va_start(ap, fmt_str);
    final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
    va_end(ap);
    if (final_n < 0 || final_n >= n)
      n += abs(final_n - n + 1);
    else
      break;
  }
  return std::string(formatted.get());
}

NFANode<std::string> NParser::compile(std::string code) {
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
  std::variant<std::string> persist;
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
      if (token.type != TOK_LITSTRING && token.type != TOK_FILESTRING) {
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
    case ParserState::Normal:
      printf("Not impl'd yet\n");
      break;
    }
  } while (!failing);
}

NFANode<std::string> NParser::compile() {
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
  NFANode<std::string> root_node{"root"};
  root_node.start = true;
  std::map<std::string, NFANode<std::string> *> node_cache;

  for (auto &it : find_leaf_rules()) {
    auto rule = std::get<Regexp *>(std::get<2>(values[it]));
    bool leading = true;
    auto node = rule->compile(node_cache, &root_node, "", leading);
    node_cache[it] = node;
  }

  for (auto &it : values) {
    if (std::get<0>(it.second) == SymbolType::Define) {
      node_cache[it.first]->final = true;
    }
  }
  root_node.optimise();
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
  std::unordered_set<CanonicalTransition<
      NFANode<T>, std::variant<char, EpsilonTransitionT, AnythingTransitionT>>>
      transitions;
  aggregate_dot(nodes, transitions);
  std::printf("%s\n", gen_dot(nodes, transitions).c_str());
}

template <typename T>
std::string NFANode<T>::gen_dot(
    std::set<NFANode<T> *, NFANodePointerComparer<T>> nodes,
    std::unordered_set<
        CanonicalTransition<NFANode<T>, std::variant<char, EpsilonTransitionT,
                                                     AnythingTransitionT>>>
        transitions) {
  std::ostringstream oss;
  constexpr auto ss_end = "\n\t";
  oss << "digraph finie_state_machine {" << ss_end;
  oss << "rankdir=LR;" << ss_end;
  oss << "size=\"8,5\";" << ss_end;
  int node_id = 0;
  std::map<NFANode<T> *, int> nodeids;
  for (auto node : nodes) {
    nodeids[node] = node_id++;
    oss << "node [shape = "
        << (node->final ? "doublecircle" : node->start ? "square" : "circle")
        << (", label = \"" +
            node->named_rule.value_or(node->state_info.value_or("<unknown>")) +
            "\\nat " + string_format("%p", node) + "\"")
        << "] LR_" << nodeids[node] << ";" << ss_end;
  }
  for (auto tr : transitions) {
    oss << "LR_" << nodeids[tr.source] << " -> LR_" << nodeids[tr.target]
        << " [ label = \""
        << (std::holds_alternative<EpsilonTransitionT>(tr.input)
                ? "<Epsilon>"
                : std::holds_alternative<AnythingTransitionT>(tr.input)
                      ? "<Any>"
                      : std::string{std::get<char>(tr.input)}.c_str())
        << " -> "
        << (tr.target->state_info.value_or(
               tr.target->named_rule.value_or("<unknown>")))
        << "\\n@" << string_format("%p", tr.target) << "\" ];" << ss_end;
  }
  oss << "}";
  return oss.str();
}

template <typename T>
void NFANode<T>::aggregate_dot(
    std::set<NFANode<T> *, NFANodePointerComparer<T>> &nodes,
    std::unordered_set<
        CanonicalTransition<NFANode<T>, std::variant<char, EpsilonTransitionT,
                                                     AnythingTransitionT>>>
        &transitions) {
  if (nodes.find(this) == nodes.end()) {
    // not yet explored
    nodes.insert(this);
    for (auto transition : get_outgoing_transitions(/* inner = */ true)) {
      transitions.insert({this, transition->target, transition->input});
      transition->target->aggregate_dot(nodes, transitions);
    }
  }
}

#ifdef TEST
int main() {
  NParser parser;
  parser.lexer = std::make_unique<NLexer>("");
  std::optional<NFANode<std::string>> root;
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
      root = parser.compile();
      root->print();
      break;
    }
    parser.repl_feed(line);
    parser.parse();
  }
}
#endif
