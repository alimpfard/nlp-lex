#include "wordtree.hpp"

extern "C" auto *make_wordtree() { return new WordTree<std::string>{}; }
extern "C" void wordtree_insert(WordTree<std::string> *tree, char const *str) {
  tree->insert({str});
}
extern "C" int wordtree_contains(WordTree<std::string> *tree, char const *str) {
  return tree->contains({str});
}
extern "C" int wordtree_get(WordTree<std::string> *tree, char const *str) {
    return tree->get({str});
}
extern "C" void wordtree_set(WordTree<std::string> *tree, char const *str, int val) {
    tree->insert({str}, val);
}
