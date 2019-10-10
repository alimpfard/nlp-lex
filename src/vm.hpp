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

static llvm::AllocaInst *createEntryBlockAlloca(llvm::Function *TheFunction,
                                                const std::string &VarName,
                                                llvm::Type *ty) {
  llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                         TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(ty, 0, VarName.c_str());
}

struct Module {
public:
  llvm::LLVMContext TheContext;
  llvm::IRBuilder<> Builder;
  std::unique_ptr<llvm::Module> TheModule;

  llvm::Function *nlex_current_f;
  llvm::Function *nlex_current_p;
  llvm::Function *nlex_restore;
  llvm::Function *nlex_next;
  llvm::Function *nlex_start;

  llvm::Function *_main = nullptr;
  llvm::BasicBlock *main_entry, *BBfinalise;

  llvm::AllocaInst *last_tag;
  llvm::AllocaInst *last_final_state_position;

  llvm::Type *input_struct_type;

  llvm::Function *main() {
    if (_main)
      return _main;
    llvm::Type *members[] = {
        llvm::Type::getInt32Ty(TheContext),
        llvm::Type::getInt32Ty(TheContext),
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0),
    };
    input_struct_type = llvm::StructType::create(members);
    llvm::Type *args[] = {llvm::PointerType::get(input_struct_type, 0)};
    llvm::FunctionType *ncf =
        llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), args, false);

    _main = llvm::Function::Create(ncf, llvm::Function::InternalLinkage,
                                   "__nlex_root", TheModule.get());
    main_entry = llvm::BasicBlock::Create(TheContext, "entry", _main);

    last_tag = createEntryBlockAlloca(
        _main, "ltag",
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0));
    last_final_state_position = createEntryBlockAlloca(
        _main, "lfinals_p",
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0));

    return _main;
  }

  Module(std::string name) : TheContext(), Builder(TheContext) {
    TheModule = std::make_unique<llvm::Module>(name, TheContext);
    llvm::FunctionType *ncf =
        llvm::FunctionType::get(llvm::Type::getInt8Ty(TheContext), {}, false);

    nlex_current_f =
        llvm::Function::Create(ncf, llvm::Function::ExternalLinkage,
                               "__nlex_current", TheModule.get());

    llvm::FunctionType *nln =
        llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), {}, false);

    nlex_next = llvm::Function::Create(nln, llvm::Function::ExternalLinkage,
                                       "__nlex_advance", TheModule.get());
    llvm::Type *args[] = {
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0)};
    llvm::FunctionType *nrs =
        llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), args, false);

    nlex_restore = llvm::Function::Create(nrs, llvm::Function::ExternalLinkage,
                                          "__nlex_restore", TheModule.get());
    llvm::FunctionType *ncp = llvm::FunctionType::get(
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0), {},
        false);

    nlex_current_p =
        llvm::Function::Create(ncp, llvm::Function::ExternalLinkage,
                               "__nlex_position", TheModule.get());
    nlex_start = llvm::Function::Create(ncp, llvm::Function::ExternalLinkage,
                                        "__nlex_true_start", TheModule.get());
  }
};

class Builder {
public:
  Module module;

  Builder(std::string mname) : module(mname) {}

  void begin() {
    // grumble grumble

    module.BBfinalise =
        llvm::BasicBlock::Create(module.TheContext, "_escape", module.main());
    module.Builder.SetInsertPoint(module.BBfinalise);
    auto istruct = module.main()->arg_begin();
    auto startm = module.Builder.CreateLoad(module.Builder.CreateInBoundsGEP(
        istruct,
        {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
         llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0)},
        "startptr"));
    module.Builder.CreateStore(
        module.Builder.CreateSub(
            module.Builder.CreateTrunc(
                module.Builder.CreateSub(
                    module.Builder.CreatePtrToInt(
                        module.Builder.CreateCall(module.nlex_current_p, {}),
                        llvm::Type::getInt64Ty(module.TheContext)),
                    module.Builder.CreatePtrToInt(
                        module.Builder.CreateCall(module.nlex_start, {}),
                        llvm::Type::getInt64Ty(module.TheContext))),
                llvm::Type::getInt32Ty(module.TheContext)),
            startm),
        module.Builder.CreateInBoundsGEP(
            istruct,
            {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                    0),
             llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                    1)},
            "length"));
    module.Builder.CreateStore(
        module.Builder.CreateLoad(module.last_tag),
        module.Builder.CreateInBoundsGEP(
            istruct,
            {
                llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module.TheContext), 0),
                llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module.TheContext), 2),
            },
            "tag"));
    module.Builder.CreateRetVoid();
  }
  void end() {
    // finish the function

    llvm::verifyFunction(*module.main());
    module.TheModule->print(llvm::errs(), nullptr);
  }

  llvm::Constant *mk_string(llvm::Module *M, llvm::LLVMContext &Context,
                            const std::string &svalue) {
    llvm::ArrayType *Ty =
        llvm::ArrayType::get(llvm::Type::getInt8Ty(Context), svalue.size() + 1);
    llvm::GlobalVariable *GV = new llvm::GlobalVariable(
        *M, Ty, true, llvm::GlobalValue::InternalLinkage,
        llvm::ConstantDataArray::getString(Context, svalue), "str");

    llvm::Value *idxs[] = {
        // idxs.push_back(
        reinterpret_cast<llvm::Value *>(
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0, false)),
        // idxs.push_back(
        reinterpret_cast<llvm::Value *>(
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0, false)),
    };
    return (llvm::ConstantExpr::getGetElementPtr(Ty, GV, idxs, "gepi"));
  }

  std::map<std::string, llvm::Constant *> registered_tags;
  llvm::Value *get_or_create_tag(std::string tag) {
    if (registered_tags.count(tag))
      return registered_tags[tag];
    return registered_tags[tag] =
               mk_string(module.TheModule.get(), module.TheContext, tag);
  }
};
} // namespace nlvm
