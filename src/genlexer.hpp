#pragma once
#include <map>
#include <optional>
#include <set>
#include <vector>

struct TagPosSpecifier {
  std::string from = "tag_pos.json";
  int gram = 2;
  std::string rule = "punc";
  std::string tval = ".";

  bool from_set = false, gram_set = false, rule_set = false, tval_set = false;
};

struct debug_offset_info {
  int line;
  int offset;
};

// specialise std::less for pair<std::string, debug_offset_info>

template <> struct std::less<std::pair<std::string, debug_offset_info>> {
  using _T = std::pair<std::string, debug_offset_info>;
  const bool operator()(const _T &lhs, const _T &rhs) const {
    return lhs.first < rhs.first;
  }
};

struct GenLexer {
  std::map<std::string, bool> options;
  std::set<std::pair<std::string, debug_offset_info>> stopwords;
  std::set<std::pair<std::string, debug_offset_info>> ignores;
  std::map<std::string, std::string> normalisations;
  std::map<std::string, std::vector<std::string>> literal_tags;
  std::vector<std::string> kdefines;
  std::optional<TagPosSpecifier> tagpos;
  int total_capturing_groups;
};
