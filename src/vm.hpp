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
  llvm::Function *nlex_feed;
  llvm::Function *nlex_next;
  llvm::Function *nlex_start;
  llvm::Function *nlex_get_utf8_length;

  llvm::GlobalVariable *nlex_match_start;
  llvm::GlobalVariable *token_value;
  llvm::GlobalVariable *token_length;

  llvm::Function *_main = nullptr;
  llvm::BasicBlock *main_entry, *BBfinalise;

  llvm::AllocaInst *last_tag;
  llvm::AllocaInst *last_final_state_position;

  llvm::Type *input_struct_type;

  void add_char_to_token(char c) {
    auto llen = Builder.CreateLoad(token_length);
    auto llenp1 = Builder.CreateAdd(
        llen, llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 1));
    Builder.CreateStore(llenp1, token_length);
    auto tvalp = Builder.CreateInBoundsGEP(
        token_value,
        {llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0), llen});
    Builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt8Ty(TheContext), (int)c),
        tvalp);
  }
  void add_value_to_token(llvm::Value *c) {
    auto llen = Builder.CreateLoad(token_length);
    auto llenp1 = Builder.CreateAdd(
        llen, llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 1));
    Builder.CreateStore(llenp1, token_length);
    auto tvalp = Builder.CreateInBoundsGEP(
        token_value,
        {llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0), llen});
    Builder.CreateStore(c, tvalp);
  }

  llvm::Function *main() {
    if (_main)
      return _main;
    llvm::Type *members[] = {
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext),
                               0),          // token value
        llvm::Type::getInt32Ty(TheContext), // token length
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext),
                               0), // token tag
    };
    input_struct_type = llvm::StructType::create(members);
    llvm::Type *args[] = {llvm::PointerType::get(input_struct_type, 0)};
    llvm::FunctionType *ncf =
        llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), args, false);

    _main = llvm::Function::Create(ncf, llvm::Function::ExternalLinkage,
                                   "__nlex_root", TheModule.get());
    main_entry = llvm::BasicBlock::Create(TheContext, "", _main);

    last_tag = createEntryBlockAlloca(
        _main, "ltag",
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0));
    last_final_state_position = createEntryBlockAlloca(
        _main, "lfinals_p",
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0));
    llvm::IRBuilder<> builder(TheContext);
    builder.SetInsertPoint(main_entry);
    builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0),
        token_length);
    builder.CreateStore(builder.CreateCall(nlex_current_p, {}),
                        nlex_match_start);
    return _main;
  }

  Module(std::string name) : TheContext(), Builder(TheContext) {
    TheModule = std::make_unique<llvm::Module>(name, TheContext);
    llvm::FunctionType *ncf =
        llvm::FunctionType::get(llvm::Type::getInt8Ty(TheContext), {}, false);

    nlex_current_f =
        llvm::Function::Create(ncf, llvm::Function::InternalLinkage,
                               "__nlex_current", TheModule.get());
    llvm::FunctionType *ucf =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext),
                                {llvm::Type::getInt8Ty(TheContext)}, false);

    nlex_get_utf8_length =
        llvm::Function::Create(ucf, llvm::Function::InternalLinkage,
                               "__nlex_utf8_length", TheModule.get());

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
    nlex_feed = llvm::Function::Create(nrs, llvm::Function::ExternalLinkage,
                                       "__nlex_feed", TheModule.get());
    llvm::FunctionType *ncp = llvm::FunctionType::get(
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0), {},
        false);

    nlex_current_p =
        llvm::Function::Create(ncp, llvm::Function::InternalLinkage,
                               "__nlex_position", TheModule.get());
    nlex_start = llvm::Function::Create(ncp, llvm::Function::InternalLinkage,
                                        "__nlex_true_start", TheModule.get());

    nlex_match_start = new llvm::GlobalVariable(
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0), false,
        llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(
            llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0)),
        "nlex_match_start");
    TheModule->getGlobalList().push_back(nlex_match_start);

    token_value = new llvm::GlobalVariable(
        llvm::ArrayType::get(llvm::Type::getInt8Ty(TheContext), 1024), false,
        llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(
            llvm::ArrayType::get(llvm::Type::getInt8Ty(TheContext), 1024)),
        "ltoken_value");
    TheModule->getGlobalList().push_back(token_value);

    token_length = new llvm::GlobalVariable(
        llvm::Type::getInt32Ty(TheContext), false,
        llvm::GlobalValue::InternalLinkage,
        llvm::Constant::getNullValue(llvm::Type::getInt32Ty(TheContext)),
        "ltoken_length");
    TheModule->getGlobalList().push_back(token_length);
  }
};

class Builder {
public:
  Module module;
  llvm::BasicBlock *first_root = nullptr;

  Builder(std::string mname) : module(mname) {}

  void begin() {

    module.BBfinalise =
        llvm::BasicBlock::Create(module.TheContext, "_escape", module.main());
    module.Builder.SetInsertPoint(module.BBfinalise);
    auto istruct = module.main()->arg_begin();
    auto stgep = module.Builder.CreateInBoundsGEP(
        istruct,
        {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
         llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0)},
        "startptr");
    module.Builder.CreateStore(
        llvm::ConstantExpr::getBitCast(
            module.token_value,
            llvm::PointerType::get(llvm::Type::getInt8Ty(module.TheContext),
                                   0)),
        stgep);
    module.Builder.CreateStore(
        module.Builder.CreateLoad(module.token_length),
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
    auto BB =
        llvm::BasicBlock::Create(module.TheContext, "", module.nlex_current_f);
    builder.SetInsertPoint(BB);
    builder.CreateRet(builder.CreateLoad(nlex_tmp_char));

    // nlex_current_p - get current position in string
    BB = llvm::BasicBlock::Create(module.TheContext, "", module.nlex_current_p);
    builder.SetInsertPoint(BB);
    builder.CreateRet(
        builder.CreateGEP(builder.CreateLoad(nlex_fed_string),
                          {builder.CreateLoad(nlex_injected_length_diff)}));

    // nlex_restore - restore position from passed in pointer
    BB = llvm::BasicBlock::Create(module.TheContext, "", module.nlex_restore);
    builder.SetInsertPoint(BB);
    builder.CreateStore(module.nlex_restore->arg_begin(), nlex_fed_string);
    builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        nlex_injected_length);
    builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        nlex_injected_length_diff);
    builder.CreateRetVoid();
    // nlex_feed - feed string to lexer
    BB = llvm::BasicBlock::Create(module.TheContext, "", module.nlex_feed);
    builder.SetInsertPoint(BB);
    builder.CreateStore(module.nlex_feed->arg_begin(), nlex_fed_string);
    builder.CreateStore(module.nlex_feed->arg_begin(), nlex_true_start);
    builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        nlex_injected_length);
    builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        nlex_injected_length_diff);
    builder.CreateRetVoid();
    // nlex_next - advance character position
    BB = llvm::BasicBlock::Create(module.TheContext, "", module.nlex_next);
    auto *uBB = llvm::BasicBlock::Create(module.TheContext, "has_inject",
                                         module.nlex_next);
    auto *pBB = llvm::BasicBlock::Create(module.TheContext, "no_inject",
                                         module.nlex_next);
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
        builder.CreateNSWAdd(
            len, llvm::ConstantInt::get(
                     llvm::Type::getInt32Ty(module.TheContext), -1)),
        nlex_injected_length);
    builder.CreateStore(
        builder.CreateGEP(builder.CreateLoad(nlex_injected),
                          {llvm::ConstantInt::get(
                              llvm::Type::getInt32Ty(module.TheContext), 1)}),
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
    auto cvv = builder.CreateLoad(fs);
    // create a select of all specified normalisations and then set
    std::map<std::string, llvm::SwitchInst *> levels;
    llvm::IRBuilder<> mbuilder(module.TheContext);
    auto BBend = llvm::BasicBlock::Create(module.TheContext, "default_escape",
                                          module.nlex_next);

    if (normalisations.size() == 0)
      builder.CreateBr(BBend);
    mbuilder.SetInsertPoint(BBend);
    mbuilder.CreateStore(cvv, nlex_tmp_char);
    // mbuilder.CreateStore(
    //     mbuilder.CreateGEP(mbuilder.CreateLoad(nlex_fed_string),
    //                        {llvm::ConstantInt::get(
    //                            llvm::Type::getInt32Ty(module.TheContext),
    //                            1)}),
    //     nlex_fed_string);
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
            mbuilder.CreateStore(
                get_or_create_tag(norm.substr(1, norm.size())),
                nlex_injected); // consume one byte and store the rest
            mbuilder.CreateStore(
                llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module.TheContext), norm.size() - 1),
                nlex_injected_length);
            mbuilder.CreateStore(llvm::ConstantInt::get(
                                     llvm::Type::getInt32Ty(module.TheContext),
                                     pnorm.second.size() - norm.size()),
                                 nlex_injected_length_diff);

            mbuilder.CreateStore( // store the first character (literal)
                llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext),
                                       (int)norm[0]),
                nlex_tmp_char);
            auto gep = mbuilder.CreateInBoundsGEP(
                fs, {llvm::ConstantInt::get(
                        llvm::Type::getInt32Ty(module.TheContext), i)});
            auto cvv = mbuilder.CreateLoad(gep);

            sw = mbuilder.CreateSwitch(cvv, BBend);
            mbuilder.SetInsertPoint(BBend);

            mbuilder.CreateStore(
                mbuilder.CreateLoad(mbuilder.CreateLoad(nlex_fed_string)),
                nlex_tmp_char);
            mbuilder.CreateStore(
                mbuilder.CreateGEP(
                    mbuilder.CreateLoad(nlex_fed_string),
                    {llvm::ConstantInt::get(
                        llvm::Type::getInt32Ty(module.TheContext), 1)}),
                nlex_fed_string);

            mbuilder.CreateRetVoid();
            levels[norm.substr(0, i)] = sw;
            continue;
          } else {

            auto gep = mbuilder.CreateInBoundsGEP(
                fs, {llvm::ConstantInt::get(
                        llvm::Type::getInt32Ty(module.TheContext), i)});
            auto cvv = mbuilder.CreateLoad(gep);
            sw = mbuilder.CreateSwitch(cvv, BBend);
          }
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
    BB = llvm::BasicBlock::Create(module.TheContext, "", module.nlex_start);
    builder.SetInsertPoint(BB);
    builder.CreateRet(builder.CreateLoad(nlex_true_start));

    // nlex_get_utf8_length - return the number of expected extra chars to
    // consume for the given starting byte in a utf8 sequence
    BB = llvm::BasicBlock::Create(module.TheContext, "",
                                  module.nlex_get_utf8_length);
    auto _4 = llvm::BasicBlock::Create(module.TheContext, "",
                                       module.nlex_get_utf8_length);
    auto _6 = llvm::BasicBlock::Create(module.TheContext, "",
                                       module.nlex_get_utf8_length);
    auto _8 = llvm::BasicBlock::Create(module.TheContext, "",
                                       module.nlex_get_utf8_length);
    auto _11 = llvm::BasicBlock::Create(module.TheContext, "",
                                        module.nlex_get_utf8_length);
    builder.SetInsertPoint(BB);
    auto arg = module.nlex_get_utf8_length->arg_begin();
    auto icmp = builder.CreateICmpULT(
        arg,
        llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext), 127));
    builder.CreateCondBr(icmp, _11, _4);
    builder.SetInsertPoint(_4);
    auto icmp_4 = builder.CreateICmpULT(
        arg,
        llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext), -33));
    builder.CreateCondBr(icmp_4, _11, _6);
    builder.SetInsertPoint(_6);
    auto icmp_6 = builder.CreateICmpULT(
        arg,
        llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext), -17));
    builder.CreateCondBr(icmp_4, _11, _8);
    builder.SetInsertPoint(_8);
    auto icmp_8 = builder.CreateICmpULT(
        arg,
        llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext), -9));
    auto sel = builder.CreateSelect(
        icmp_8,
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 3),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0));
    builder.CreateBr(_11);
    builder.SetInsertPoint(_11);
    auto phi = builder.CreatePHI(llvm::Type::getInt32Ty(module.TheContext), 4);
    phi->addIncoming(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        BB);
    phi->addIncoming(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 1),
        _4);
    phi->addIncoming(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 2),
        _6);
    phi->addIncoming(sel, _8);
    builder.CreateRet(phi);
  } // namespace nlvm
  void end() {
    // finish the function
    if (first_root) {
      llvm::IRBuilder<> builder(module.TheContext);
      builder.SetInsertPoint(module.main_entry);
      builder.CreateBr(first_root);
    }

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
