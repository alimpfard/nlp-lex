#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

namespace WordTreeActions {
struct store_value_tag {
};
}; // namespace WordTreeActions

template<typename StringT, typename MetadataT = int,
    typename CollectionT = std::set<StringT>,
    typename PairCollectionT = std::set<std::pair<StringT, MetadataT>>,
    typename _CharT = typename StringT::value_type>

class WordTree {
    using CharT = _CharT;
    using value_type = CharT;

    constexpr static CharT
        EOW {}; // value-constructed Char type should mark end of word

    struct WordTreeNode {
    public:
        std::map<CharT, std::shared_ptr<struct WordTreeNode>> elements;
        MetadataT metadata = {};

        WordTreeNode()
            : elements({})
        {
        }

        auto begin() const { return elements.cbegin(); }
        auto end() const { return elements.cend(); }
        auto has_transition(CharT t) const { return elements.count(t) > 0; }
    };

public:
    std::shared_ptr<WordTreeNode> root_node;
    WordTree(CollectionT words)
    {
        root_node = std::make_shared<WordTreeNode>();
        for (auto word : words)
            insert(word);
    }
    WordTree(CollectionT words, WordTreeActions::store_value_tag tag)
    {
        root_node = std::make_shared<WordTreeNode>();
        for (auto word : words)
            insert(word, word);
    }
    WordTree(const PairCollectionT& words)
    {
        root_node = std::make_shared<WordTreeNode>();
        for (auto [word, tag] : words)
            insert(word, tag);
    }
    WordTree() { root_node = std::make_shared<WordTreeNode>(); }

    ~WordTree() = default;

    void insert(StringT value)
    {
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
            _root->elements[EOW] = std::make_shared<struct WordTreeNode>();
    }
    void insert(StringT value, MetadataT val)
    {
        auto _root = root_node;
        for (auto c : value) {
            if (_root->elements.count(c)) {
                _root = _root->elements[c];
            } else {
                _root->elements[c] = std::make_shared<WordTreeNode>();
                _root = _root->elements[c];
            }
        }
        std::shared_ptr<struct WordTreeNode> end;
        if (!_root->elements.count(EOW))
            _root->elements[EOW] = std::make_shared<struct WordTreeNode>();
        end = _root->elements[EOW];
        end->metadata = val;
    }
    bool contains(StringT value) const
    {
        auto _root = root_node;
        for (auto c : value) {
            if (!_root->elements.count(c))
                return false;
        }
        if (!_root->elements.count(EOW))
            return false;
        return true;
    }
    bool get(StringT value, MetadataT* val) const
    {
        auto _root = root_node;
        for (auto c : value) {
            if (!_root->elements.count(c))
                return false;
        }
        if (!_root->elements.count(EOW))
            return false;
        *val = _root->elements[EOW]->metadata;
        return true;
    }
};
