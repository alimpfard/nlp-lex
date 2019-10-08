#pragma once

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace nlvm {

struct Module {
public:
  llvm::LLVMContext TheContext;
  llvm::IRBuilder<> Builder;
  std::unique_ptr<llvm::Module> TheModule;

  llvm::Function *nlex_current_f;

  llvm::Function *_main = nullptr;
  llvm::BasicBlock *main_entry;
  llvm::Function *main() {
    if (_main)
      return _main;

    llvm::FunctionType *ncf =
        llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), {}, false);

    _main = llvm::Function::Create(ncf, llvm::Function::InternalLinkage,
                                   "__nlex_root", TheModule.get());
    main_entry = llvm::BasicBlock::Create(TheContext, "entry", _main);
    return _main;
  }

  Module(std::string name) : TheContext(), Builder(TheContext) {
    TheModule = std::make_unique<llvm::Module>(name, TheContext);
    llvm::FunctionType *ncf =
        llvm::FunctionType::get(llvm::Type::getInt8Ty(TheContext), {}, false);

    nlex_current_f =
        llvm::Function::Create(ncf, llvm::Function::ExternalLinkage,
                               "__nlex_current", TheModule.get());
  }
};

class Builder {
public:
  Module module;

  Builder(std::string mname) : module(mname) {}

  void begin() {
    // grumble grumble
  }
  void end() {
    // finish the function

    llvm::verifyFunction(*module.main());
    module.TheModule->print(llvm::errs(), nullptr);
  }
};
} // namespace nlvm
