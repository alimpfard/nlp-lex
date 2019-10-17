#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

template <typename StringT, typename CollectionT = std::set<StringT>>
class WordTree {
  using CharT = typename StringT::value_type;
  using value_type = CharT;

  constexpr static CharT
      EOW{}; // value-constructed Char type should mark end of word

  struct WordTreeNode {
  public:
    std::map<CharT, std::shared_ptr<struct WordTreeNode>> elements;

    WordTreeNode() : elements({}) {}

    auto begin() const { return elements.cbegin(); }
    auto end() const { return elements.cend(); }
  };

public:
  std::shared_ptr<WordTreeNode> root_node;
  WordTree(CollectionT words) {
    root_node = std::make_shared<WordTreeNode>();
    for (auto word : words)
      insert(word);
  }

  ~WordTree() = default;

  void insert(StringT value) {
    auto _root = root_node;
    for (auto c : value) {
      if (_root->elements.count(c)) {
        _root = _root->elements[c];
      } else {
        _root->elements[c] = std::make_shared<WordTreeNode>();
        _root = _root->elements[c];
      }
    }
    if (!_root->elements.count(EOW))
      _root->elements[EOW] = {};
  }
};