#pragma once
#include "llvm/ADT/Triple.h"
#include "llvm/Target/TargetMachine.h"
#include <string>

namespace nlvm {
struct TargetTriple {
  llvm::Triple triple;
  llvm::Reloc::Model reloc_model;
  std::string cpu = "", features = "";
  bool _cross = false;
  bool _write_ll = false;
  bool library = false;
};
} // namespace nlvm
