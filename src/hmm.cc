// #include <jansson.h>
#include "hmm.hpp"
#include "cassert"
#include "serialise.hpp"
#include <math.h>
#include <array>

namespace nlex {
namespace POSTag {

class Node {
public:
  std::string prev;
  double value;

  Node(std::string prev, double value) : prev(std::move(prev)), value(value) {}

  Node(const Node &) = default;
  Node() : prev({}), value(-1000000) {}
};

struct vnode {
  std::string first;
  Node second;
  vnode(const std::pair<std::string, Node> &&f)
      : first(f.first), second(f.second) {}
  vnode(const std::pair<std::string, Node> &f)
      : first(f.first), second(f.second) {}
  vnode(const std::string &first, const Node &second)
      : first(first), second(second) {}

  explicit operator std::pair<std::string, Node>() { return {first, second}; }
};

constexpr auto DEFAULT_SCORE{-100};

std::vector<std::string> viterbi(TType &S, TType &T,
                                 std::vector<std::string> &v) {
  std::vector<std::map<std::string, Node>> tbl;

  for (auto i = 0; i < v.size(); ++i)
    tbl.push_back({});

  for (auto &s : S) {
    auto val = s.second.find(v[0]);
    double value;
    if (val == s.second.end())
      value = DEFAULT_SCORE;
    else
      value = val->second;

    tbl[0][s.first] = Node{"", value};
  }
  for (int i = 1; i < v.size(); ++i) {
    for (auto &s : S) {
      auto &cur = s.first;
      Node node;

      for (auto &p : S) {
        auto &mp = T[p.first];
        auto entry = mp.find(cur);
        auto score = tbl[i - 1][p.first].value +
                     (entry == mp.end() ? DEFAULT_SCORE : entry->second);
        if (node.value < score) {
          node.prev = p.first;
          node.value = score;
        }
      }
      auto &mp = S[cur];
      auto it = mp.find(v[i]);
      node.value += (it == mp.end() ? DEFAULT_SCORE : it->second);
      tbl[i][cur] = node;
    }
  }
  auto t = v.size() - 1;
  std::vector<std::string> L;
  L.resize(v.size());
  const auto &tblt = tbl[t];
  L[t] = std::accumulate(tblt.begin(), tblt.end(), vnode{"", Node()},
                         [&](auto &acc, auto &x) {
                           return acc.second.value > x.second.value ? acc
                                                                    : vnode{x};
                         })
             .first;
  while (0 < t) {
    L[t - 1] = tbl[t][L[t]].prev;
    --t;
  }
  return L;
}
#ifndef ONLY_VITERBI_INCLUDE
static TType T{};
static TType S{};
static std::string prev = "";

void logprob(typename decltype(T)::mapped_type &q) {
  auto n{0};
  std::for_each(q.begin(), q.end(), [&](auto &x) -> void { n += x.second; });
  std::for_each(q.begin(), q.end(),
                [&](auto &x) -> void { x.second = log(x.second / n); });
}

void logprob1(typename decltype(T)::value_type &q) { logprob(q.second); }

namespace detail {
class Line : public std::string {
  friend std::istream &operator>>(std::istream &is, Line &line) {
    return std::getline(is, line);
  }
};
} // namespace detail

void train_line(const std::string &sxv) {
  std::string s{sxv.begin(), sxv.end()};
  auto pos = s.find('\t');
  if (pos == s.npos)
    return;
  if (pos == 0) {
    std::cout << "POS = 0 in " << s << '\n';
    return;
  }
  auto token = s.substr(0, pos);
  auto label = s.substr(pos + 1);
  auto &ss = S[label];
  auto &sv = ss[token];
  ++sv;
  if (prev.size() != 0) {
    auto &ss = T[prev];
    auto &sv = ss[label];
    ++sv;
  }
  prev = label;
}

template <class Func> void read_lines(std::istream &is, Func dest) {
  typedef std::istream_iterator<detail::Line> InIt;
  std::for_each(InIt(is), InIt(), dest);
}

static bool trained_once = false;

std::string train(std::string filename) {
  assert(!trained_once && "POSTag::train called more than once");

  trained_once = true;

  std::ifstream fst{filename.c_str()};
  read_lines(fst, train_line);
  std::for_each(S.begin(), S.end(), logprob1);
  std::for_each(T.begin(), T.end(), logprob1);

  std::array<TType, 2> arr{T, S};
  std::ostringstream oss;
  oss << bits(arr);
  return oss.str();
}
#endif // ONLY_VITERBI_INCLUDE

} // namespace POSTag
} // namespace nlex