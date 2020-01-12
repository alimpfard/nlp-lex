#pragma once
#include <string>
#include "llvm/ADT/Triple.h"
#include "llvm/Target/TargetMachine.h"

namespace nlvm {
    struct TargetTriple {
        llvm::Triple triple;
        llvm::Reloc::Model reloc_model;
        bool _cross = false;
        bool _write_ll = false;
    };
}