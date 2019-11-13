#pragma once
#include <map>
#include <set>
#include <vector>

struct GenLexer {
  std::map<std::string, bool> options;
  std::set<std::string> stopwords;
  std::set<std::string> ignores;
  std::map<std::string, std::string> normalisations;
  std::map<std::string, std::vector<std::string>> literal_tags;
};