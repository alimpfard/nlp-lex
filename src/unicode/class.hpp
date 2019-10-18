// data
#include "classes.hpp"

// logic
namespace UnicodeClasses {
constexpr std::string_view all_of_class(std::string_view name) {
  if (name == "L")
    return Class_L;
  if (name == "M")
    return Class_M;
  if (name == "N")
    return Class_N;
  if (name == "P")
    return Class_P;
  if (name == "S")
    return Class_S;
  if (name == "Z")
    return Class_Z;
  if (name == "C")
    return Class_C;

  if (name == "Ll")
    return Class_Ll;
  if (name == "Lm")
    return Class_Lm;
  if (name == "Lt")
    return Class_Lt;
  if (name == "Lu")
    return Class_Lu;
  if (name == "Lo")
    return Class_Lo;
  if (name == "Mc")
    return Class_Mc;
  if (name == "Me")
    return Class_Me;
  if (name == "Mn")
    return Class_Mn;
  if (name == "Nd")
    return Class_Nd;
  if (name == "Nl")
    return Class_Nl;
  if (name == "No")
    return Class_No;
  if (name == "Pc")
    return Class_Pc;
  if (name == "Pd")
    return Class_Pd;
  if (name == "Pi")
    return Class_Pi;
  if (name == "Pf")
    return Class_Pf;
  if (name == "Ps")
    return Class_Ps;
  if (name == "Pe")
    return Class_Pe;
  if (name == "Po")
    return Class_Po;
  if (name == "Sc")
    return Class_Sc;
  if (name == "Sk")
    return Class_Sk;
  if (name == "Sm")
    return Class_Sm;
  if (name == "So")
    return Class_So;
  if (name == "Zl")
    return Class_Zl;
  if (name == "Zp")
    return Class_Zp;
  if (name == "Zs")
    return Class_Zs;
  if (name == "Cc")
    return Class_Cc;
  if (name == "Cf")
    return Class_Cf;
  if (name == "Cn")
    return Class_Cn;
  if (name == "Co")
    return Class_Co;
  if (name == "Words")
    return Words;
  if (name == "Digits")
    return Digits;
  if (name == "Whitespaces")
    return Whitespaces;
  return "";
}
} // namespace UnicodeClasses
