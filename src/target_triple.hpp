#pragma once
#include <string>
#include "llvm/ADT/Triple.h"

namespace nlvm {
    struct TargetTriple {
        llvm::Triple triple;
        bool _cross = false;
        bool _write_ll = false;
    };
}