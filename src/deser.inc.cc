#include "hmm.hpp"
#include "serialise.hpp"
#include <list>
#include <queue>
#include <array>
#include <cstring>

#define ONLY_VITERBI_INCLUDE
#include "hmm.cc"
#undef ONLY_VITERBI_INCLUDE

static std::array<nlex::POSTag::TType, 2> _m_value;
static bool _m_initd = false;
extern "C" char const __nlex_postag_data;
extern "C" int const __nlex_postag_data_length;
extern "C" char const __nlex_ptag;
extern "C" void __nlex_root(void *);

struct _sresult {
  char const *start;
  int length;
  char const *tag;
  char errc;
  unsigned char metadata;
  char *POS;
};

static std::queue<std::pair<bool, std::list<_sresult>>> data_store{};
static bool last_entry_complete = false;

char* strndup(char const* ptr, size_t length) noexcept {
    char* p = (char*) malloc(length + 1);
    memcpy(p, ptr, length);
    p[length] = 0;
    return p;
}

extern "C" nlex::POSTag::TType *__nlex_get_deser_postag_TS() noexcept {
  if (_m_initd)
    return _m_value.data();
  std::istringstream iss{
      std::string(&__nlex_postag_data, __nlex_postag_data_length)};
  iss >> bits(_m_value);
  _m_initd = true;
  return _m_value.data();
}
static bool _m_toplevel = true;

extern "C" void __nlex_apply_postag(_sresult *val) {
  bool vv = data_store.size() == 0;
  bool topl = _m_toplevel;
  if (val->errc == 0 && val->length > 0) {
    if (last_entry_complete) {
      last_entry_complete = false;
      std::list<_sresult> list{};
      list.push_back({strndup(val->start, val->length), val->length, val->tag,
                      val->errc, val->metadata, nullptr});
      data_store.push({false, list});
    } else {
      if (vv)
        data_store.push({false, {}});
      data_store.back().second.push_back({strndup(val->start, val->length),
                                          val->length, val->tag, val->errc,
                                          val->metadata, nullptr});
    }
    if (strcmp(val->tag, &__nlex_ptag) == 0)
      last_entry_complete = true;
    else if (!last_entry_complete) {
      _m_toplevel = false;
      __nlex_root(val);
      _m_toplevel = topl;
    }
  }
  if (!_m_toplevel)
    return;

  if (data_store.size() == 0)
    return;

  // now we have at least one sentence ready
  // we will simply tag it (if it isn't already) and return the first
  auto &sentv = data_store.front();
  auto &sent = sentv.second;

  if (!sentv.first) {
    auto *TS = __nlex_get_deser_postag_TS();
    std::vector<std::string> inp;
    for (auto &s : sent)
      inp.push_back(std::string(s.start, s.length));
    auto rvec = nlex::POSTag::viterbi(TS[1], TS[0], inp);
    auto it = sent.begin();
    for (auto i{0}; i < rvec.size(); ++i, std::advance(it, 1))
      it->POS = strdup(rvec[i].c_str());
    sentv.first = true;
  }
  *val = sent.front();
  sent.pop_front();
  if (sent.size() == 0)
    data_store.pop();
}
