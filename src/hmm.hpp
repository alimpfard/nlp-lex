#include <algorithm>
#include <cstring>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <utility>
#include <vector>

namespace nlex {
namespace POSTag {

using TType = std::map<std::string, std::map<std::string, double>>;

std::string train(std::string input_filename);

template <class Func> void read_lines(std::istream &s, Func dest);

std::vector<std::string> viterbi(TType &s, TType &T,
                                 std::vector<std::string> &v);

} // namespace POSTag
} // namespace nlex