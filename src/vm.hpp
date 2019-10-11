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
        llvm::Function::Create(ncf, llvm::Function::InternalLinkage,
                               "__nlex_current", TheModule.get());

    llvm::FunctionType *nln =
        llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), {}, false);

    nlex_next = llvm::Function::Create(nln, llvm::Function::InternalLinkage,
                                       "__nlex_advance", TheModule.get());
    llvm::Type *args[] = {
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0)};
    llvm::FunctionType *nrs =
        llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), args, false);

    nlex_restore = llvm::Function::Create(nrs, llvm::Function::InternalLinkage,
                                          "__nlex_restore", TheModule.get());
    llvm::FunctionType *ncp = llvm::FunctionType::get(
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0), {},
        false);

    nlex_current_p =
        llvm::Function::Create(ncp, llvm::Function::InternalLinkage,
                               "__nlex_position", TheModule.get());
    nlex_start = llvm::Function::Create(ncp, llvm::Function::InternalLinkage,
                                        "__nlex_true_start", TheModule.get());
  }
};

class Builder {
public:
  Module module;

  Builder(std::string mname) : module(mname) {}

  void begin() {
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
  void prepare(std::map<std::string, std::string> normalisations,
               std::set<std::string> stopwords) {
    // create global values
    auto nlex_fed_string = new llvm::GlobalVariable(
        llvm::PointerType::get(llvm::Type::getInt8Ty(module.TheContext), 0),
        false, llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(llvm::PointerType::get(
            llvm::Type::getInt8Ty(module.TheContext), 0)),
        "nlex_fed_string");
    module.TheModule->getGlobalList().push_back(nlex_fed_string);
    auto nlex_true_start = new llvm::GlobalVariable(
        llvm::PointerType::get(llvm::Type::getInt8Ty(module.TheContext), 0),
        false, llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(llvm::PointerType::get(
            llvm::Type::getInt8Ty(module.TheContext), 0)),
        "nlex_true_start");
    module.TheModule->getGlobalList().push_back(nlex_true_start);
    auto nlex_tmp_char = new llvm::GlobalVariable(
        llvm::Type::getInt8Ty(module.TheContext), false,
        llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(llvm::Type::getInt8Ty(module.TheContext)),
        "nlex_tmp_char");
    module.TheModule->getGlobalList().push_back(nlex_tmp_char);
    auto nlex_injected = new llvm::GlobalVariable(
        llvm::PointerType::get(llvm::Type::getInt8Ty(module.TheContext), 0),
        false, llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(llvm::PointerType::get(
            llvm::Type::getInt8Ty(module.TheContext), 0)),
        "nlex_injected");
    module.TheModule->getGlobalList().push_back(nlex_injected);
    auto nlex_injected_length = new llvm::GlobalVariable(
        llvm::Type::getInt32Ty(module.TheContext), false,
        llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(llvm::Type::getInt8Ty(module.TheContext)),
        "nlex_injected_length");
    module.TheModule->getGlobalList().push_back(nlex_injected_length);
    auto nlex_injected_length_diff = new llvm::GlobalVariable(
        llvm::Type::getInt32Ty(module.TheContext), false,
        llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(llvm::Type::getInt8Ty(module.TheContext)),
        "nlex_injected_length_diff");
    module.TheModule->getGlobalList().push_back(nlex_injected_length_diff);

    // create "library" functions
    llvm::IRBuilder<> builder(module.TheContext);

    // nlex_current_f - get current character
    auto BB = llvm::BasicBlock::Create(module.TheContext, "entry",
                                       module.nlex_current_f);
    builder.SetInsertPoint(BB);
    builder.CreateRet(builder.CreateLoad(nlex_tmp_char));

    // nlex_current_p - get current position in string
    BB = llvm::BasicBlock::Create(module.TheContext, "entry",
                                  module.nlex_current_p);
    builder.SetInsertPoint(BB);
    builder.CreateRet(
        builder.CreateGEP(builder.CreateLoad(nlex_fed_string),
                          {builder.CreateLoad(nlex_injected_length_diff)}));

    // nlex_restore - restore position from passed in pointer
    BB = llvm::BasicBlock::Create(module.TheContext, "entry",
                                  module.nlex_restore);
    builder.SetInsertPoint(BB);
    builder.CreateStore(module.nlex_restore->arg_begin(), nlex_fed_string);
    builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        nlex_injected_length);
    builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        nlex_injected_length_diff);
    builder.CreateRetVoid();

    // nlex_next - advance character position
    BB = llvm::BasicBlock::Create(module.TheContext, "entry", module.nlex_next);
    auto *uBB =
        llvm::BasicBlock::Create(module.TheContext, "entry", module.nlex_next);
    auto *pBB =
        llvm::BasicBlock::Create(module.TheContext, "entry", module.nlex_next);
    builder.SetInsertPoint(BB);
    auto len = builder.CreateLoad(nlex_injected_length);
    builder.CreateCondBr(
        builder.CreateICmpSGT(
            len, llvm::ConstantInt::get(
                     llvm::Type::getInt32Ty(module.TheContext), 0)),
        uBB, pBB);
    // inject
    builder.SetInsertPoint(uBB);
    builder.CreateStore(
        builder.CreateNSWAdd(len,
                             llvm::ConstantInt::get(
                                 llvm::Type::getInt32Ty(module.TheContext), 1)),
        nlex_injected_length);
    builder.CreateStore(
        builder.CreateGEP(builder.CreateLoad(nlex_injected),
                          {llvm::ConstantInt::get(
                              llvm::Type::getInt32Ty(module.TheContext), -1)}),
        nlex_injected);
    builder.CreateStore(builder.CreateLoad(builder.CreateLoad(nlex_injected)),
                        nlex_tmp_char);
    builder.CreateRetVoid();
    // no inject
    BB = pBB;
    builder.SetInsertPoint(BB);
    auto fs = builder.CreateLoad(nlex_fed_string);
    auto gep = builder.CreateInBoundsGEP(
        fs,
        {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 1)});
    builder.CreateStore(gep, nlex_fed_string);
    auto cvv = builder.CreateLoad(gep);
    // create a select of all specified normalisations and then set
    std::map<std::string, llvm::SwitchInst *> levels;
    llvm::IRBuilder<> mbuilder(module.TheContext);
    auto BBend = llvm::BasicBlock::Create(module.TheContext, "default_escape",
                                          module.nlex_next);
    mbuilder.SetInsertPoint(BBend);
    mbuilder.CreateRetVoid();
    for (auto pnorm : normalisations) {
      mbuilder.SetInsertPoint(BB);
      auto norm = pnorm.first;
      for (int i = 0; i <= norm.size(); i++) {
        if (!levels.count(norm.substr(0, i))) {
          // we don't have it in the cache

          // create a toplevel block
          llvm::SwitchInst *sw;
          if (i == norm.size()) {

            auto BBend = llvm::BasicBlock::Create(
                module.TheContext, "default_escape-" + norm, module.nlex_next);
            // match, TODO: call inject
            mbuilder.CreateStore(get_or_create_tag(norm), nlex_injected);
            mbuilder.CreateStore(
                llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module.TheContext), norm.size()),
                nlex_injected_length);
            mbuilder.CreateStore(llvm::ConstantInt::get(
                                     llvm::Type::getInt32Ty(module.TheContext),
                                     pnorm.second.size() - norm.size()),
                                 nlex_injected_length_diff);
            sw = mbuilder.CreateSwitch(cvv, BBend);
            mbuilder.SetInsertPoint(BBend);
            mbuilder.CreateRetVoid();
            levels[norm.substr(0, i)] = sw;
            continue;
          } else
            sw = mbuilder.CreateSwitch(cvv, BBend);
          auto cBB = llvm::BasicBlock::Create(
              module.TheContext, norm.substr(0, i), module.nlex_next);
          sw->addCase(
              llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext),
                                     (int)norm[i]),
              cBB);
          levels[norm.substr(0, i)] = sw;
          mbuilder.SetInsertPoint(cBB);
        } else {
          // that switch already exists, add a case to it
          auto sw = levels[norm.substr(0, i)];
          llvm::BasicBlock *cBB;
          auto val = llvm::ConstantInt::get(
              llvm::Type::getInt8Ty(module.TheContext), (int)norm[i]);
          auto ci = sw->findCaseValue(val);
          if (ci == sw->case_default()) {
            cBB = llvm::BasicBlock::Create(module.TheContext, norm.substr(0, i),
                                           module.nlex_next);
            sw->addCase(val, cBB);
          } else
            cBB = sw->getSuccessor(ci->getSuccessorIndex());
          mbuilder.SetInsertPoint(cBB);
        }
      }
    }
    // nlex_start - return the true start of the fed string
    BB =
        llvm::BasicBlock::Create(module.TheContext, "entry", module.nlex_start);
    builder.SetInsertPoint(BB);
    builder.CreateRet(builder.CreateLoad(nlex_true_start));
  } // namespace nlvm
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
}; // namespace nlvm
} // namespace nlvm
