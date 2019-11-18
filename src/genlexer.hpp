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

struct GenLexer {
  std::map<std::string, bool> options;
  std::set<std::string> stopwords;
  std::set<std::string> ignores;
  std::map<std::string, std::string> normalisations;
  std::map<std::string, std::vector<std::string>> literal_tags;
  std::optional<TagPosSpecifier> tagpos;
};
