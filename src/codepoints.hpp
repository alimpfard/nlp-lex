#pragma once

class Codepoints {
  char const *p;
  size_t length;

public:
  Codepoints(char const *sp, int len) : p(sp), length(len) {}
  void operator++() {
    p += length;
    fix_length();
  }
  std::string operator*() { return std::string{p, length}; }
  bool operator!=(const Codepoints &other) const { return other.p != p; }

  void fix_length() { length = getlength(p); }
  inline const static int getlength(const char *p) {
    unsigned char sp = *p;
    if (sp <= 0b01111111)
      return 1;
    else if (sp <= 0b11011111)
      return 2;
    else if (sp <= 0b11101111)
      return 3;
    else if (sp <= 0b11110111)
      return 4;
    else
      return 5; // not utf-8
  }
};

class codepoints {
  char const *sp;
  int length;

public:
  codepoints(std::string s) : sp(strdup(s.c_str())), length(s.size()) {}
  ~codepoints() { free(const_cast<char *>(sp)); }

  Codepoints begin() {
    auto cp = Codepoints(sp, 0);
    cp.fix_length();
    return cp;
  }
  Codepoints end() { return Codepoints(sp + length, 0); }

  static bool is_single_code_point(std::string ss) {
    auto sl = Codepoints::getlength(ss.c_str());
    return ss.size() <= sl;
  }
};
