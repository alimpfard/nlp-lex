#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/DebugInfo/DIContext.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

namespace nlvm {
struct BaseModule {
public:
  llvm::LLVMContext TheContext;
  std::unique_ptr<llvm::Module> TheModule;
  std::unique_ptr<llvm::legacy::PassManager> TheMPM;
  // std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;
  BaseModule(std::string name = "") : TheContext() {
    TheModule = std::make_unique<llvm::Module>(name, TheContext);
  }
};
} // namespace nlvm
