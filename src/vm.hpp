#pragma once

#include "genlexer.hpp"
#include "termdisplay.hpp"
#include "wordtree.hpp"
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
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
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
#include <vector>

template <typename T>
typename T::mapped_type get(T value, typename T::key_type key) {
  auto v = value.find(key);
  if (v == value.end())
    return {};
  return v->second;
}

extern Display::SingleLineTermStatus slts;

namespace nlvm {

static void debugPrintValue(llvm::Value *v) { v->print(llvm::errs(), true); }
static void debugPrintType(llvm::Value *v) {
  v->getType()->print(llvm::errs(), true);
}

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

  llvm::BasicBlock *start;

  llvm::DIBuilder *DBuilder;
  llvm::DICompileUnit *TheCU;
  llvm::DIFile *DIFile;
  std::vector<llvm::DIScope *> LexicalDebugBlocks;

  llvm::Function *nlex_current_f;
  llvm::Function *nlex_current_p;
  llvm::Function *nlex_distance;
  llvm::Function *nlex_restore;
  llvm::Function *nlex_feed;
  llvm::Function *nlex_next;
  llvm::Function *nlex_start;
  llvm::Function *nlex_get_utf8_length;
  llvm::Function *nlex_skip;

  llvm::GlobalVariable *nlex_match_start;
  llvm::GlobalVariable *token_value;
  llvm::GlobalVariable *token_length;
  llvm::GlobalVariable *nlex_fed_string;

  llvm::Function *_main = nullptr;
  llvm::BasicBlock *main_entry, *BBfinalise = nullptr;

  llvm::AllocaInst *last_tag;
  llvm::AllocaInst *last_final_state_position;
  llvm::AllocaInst *chars_since_last_final;
  llvm::AllocaInst *anything_matched;
  llvm::AllocaInst *nlex_errc;

  llvm::Type *input_struct_type;

  llvm::raw_ostream *outputv;

  void emitLocationL(int l, llvm::IRBuilder<> &builder) {
    llvm::DIScope *Scope;
    if (LexicalDebugBlocks.empty())
      Scope = TheCU;
    else
      Scope = LexicalDebugBlocks.back();
    builder.SetCurrentDebugLocation(llvm::DebugLoc::get(l, 0, Scope));
  }
  template <typename T> void emitLocation(T *node, llvm::IRBuilder<> &builder) {
    llvm::DIScope *Scope;
    if (LexicalDebugBlocks.empty())
      Scope = TheCU;
    else
      Scope = LexicalDebugBlocks.back();
    builder.SetCurrentDebugLocation(
        llvm::DebugLoc::get(node ? node->debug_info.lineno : 0,
                            node ? node->debug_info.offset : 0, Scope));
  }
  template <typename T> void emitLocation(T *node) {
    emitLocation(node, Builder);
  }

  template <typename T> void enterScope(T *node) {
    llvm::DIScope *Scope = TheCU;
    if (!LexicalDebugBlocks.empty())
      Scope = LexicalDebugBlocks.back();
    Scope = llvm::DILexicalBlock::get(TheContext, Scope, DIFile,
                                      node->debug_info.lineno,
                                      node->debug_info.offset);
    LexicalDebugBlocks.push_back(Scope);
  }
  void exitScope() {
    assert(!LexicalDebugBlocks.empty() && "Extra block pop");
    LexicalDebugBlocks.pop_back();
  }

  llvm::DIType *input_struct_ditype = nullptr;
  llvm::DIType *getInputStructDiType() {
    if (input_struct_ditype)
      return input_struct_ditype;

    llvm::SmallVector<llvm::Metadata *, 5> diinputtype;
    int offset = 0;
    char *names[] = {"token_value", "token_length", "tag", "error_code",
                     "metadata"};
    llvm::DIType *i8p = DBuilder->createBasicType(
        "char*", TheModule->getDataLayout().getPointerSizeInBits(),
        llvm::dwarf::DW_ATE_address);
    llvm::DIType *i8 =
        DBuilder->createBasicType("char", 8, llvm::dwarf::DW_ATE_signed_char);
    llvm::DIType *i32 =
        DBuilder->createBasicType("int", 32, llvm::dwarf::DW_ATE_signed);

    llvm::DIType *baseTypes[] = {i8p, i32, i8p, i8, i8};
    for (auto i = 0; i < 5; i++) {
      auto align = TheModule->getDataLayout().getABITypeAlignment(
          input_struct_type->getStructElementType(i));
      diinputtype.push_back(DBuilder->createMemberType(
          TheCU,
          names[i], // name
          DIFile,   // file
          0,        // line number
          TheModule->getDataLayout().getTypeAllocSize(
              input_struct_type->getStructElementType(i)) *
              8,                           // size (bits)
          align * 8,                       // align (bits)
          offset * 8,                      // offset (bits)
          llvm::DINode::DIFlags::FlagZero, // flags
          baseTypes[i]));                  // derived from
      offset += align;
    }
    input_struct_ditype = DBuilder->createStructType(
        TheCU, "sreturn_t", DIFile, 0,
        TheModule->getDataLayout().getTypeAllocSizeInBits(input_struct_type),
        TheModule->getDataLayout().getTypeAllocSizeInBits(input_struct_type),
        (llvm::DINode::DIFlags)0, nullptr,
        DBuilder->getOrCreateArray(diinputtype));
    return input_struct_ditype;
  }
  llvm::DISubroutineType *createFunctionType() {
    llvm::SmallVector<llvm::Metadata *, 2> EltTys;

    // Add the result type.
    EltTys.push_back(nullptr);
    EltTys.push_back(getInputStructDiType());

    return DBuilder->createSubroutineType(
        DBuilder->getOrCreateTypeArray(EltTys));
  }
  llvm::DISubroutineType *createNullaryFunctionType() {
    llvm::SmallVector<llvm::Metadata *, 1> EltTys;

    // Add the result type.
    EltTys.push_back(nullptr);

    return DBuilder->createSubroutineType(
        DBuilder->getOrCreateTypeArray(EltTys));
  }

  llvm::DISubprogram *enterSubprogramN(std::string name) {
    llvm::DIScope *FContext = TheCU;
    unsigned LineNo = 0;
    unsigned ScopeLine = 0;
    llvm::DISubprogram *SP = DBuilder->createFunction(
        FContext, name, "", DIFile, LineNo, createNullaryFunctionType(),
        ScopeLine, llvm::DINode::DIFlags::FlagZero,
        llvm::DISubprogram::toSPFlags(true, true, true, 0, false));
    DBuilder->finalizeSubprogram(SP);
    LexicalDebugBlocks.push_back(SP);
    return SP;
  }

  llvm::DISubprogram *enterSubprogram(llvm::Function *fn) {
    llvm::DIScope *FContext = TheCU;
    unsigned LineNo = 0;
    unsigned ScopeLine = 0;
    llvm::DISubprogram *SP = DBuilder->createFunction(
        FContext, fn->getName(), "", DIFile, LineNo, createFunctionType(),
        ScopeLine, llvm::DINode::DIFlags::FlagZero,
        llvm::DISubprogram::toSPFlags(true, true, true, 0, fn == _main));
    for (llvm::Argument &arg : fn->args()) {
      llvm::DILocalVariable *D = DBuilder->createParameterVariable(
          SP, arg.getName(), arg.getArgNo(), DIFile, LineNo,
          getInputStructDiType(), true);

      DBuilder->insertDeclare(&arg, D, DBuilder->createExpression(),
                              llvm::DebugLoc::get(LineNo, 0, SP),
                              &fn->getEntryBlock());
    }
    DBuilder->finalizeSubprogram(SP);
    LexicalDebugBlocks.push_back(SP);
    return SP;
  }
  void add_char_to_token(char c, llvm::IRBuilder<> &Builder) {
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

  void add_char_to_token(char c) { add_char_to_token(c, Builder); }
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

  llvm::Function *mkfunc(bool clear = true, std::string name = "__nlex_root",
                         bool toplevels = true, bool nullary = false) {
    llvm::Function *_main;
    llvm::ArrayRef<llvm::Type *> args = {
        llvm::PointerType::get(input_struct_type, 0)};
    llvm::ArrayRef<llvm::Type *> nnargs = {};
    llvm::FunctionType *ncf = llvm::FunctionType::get(
        llvm::Type::getVoidTy(TheContext), nullary ? nnargs : args, false);

    _main = llvm::Function::Create(ncf, llvm::Function::ExternalLinkage, name,
                                   TheModule.get());
    auto main_entry = llvm::BasicBlock::Create(TheContext, "", _main);
    if (!toplevels)
      return _main;

    last_tag = createEntryBlockAlloca(
        _main, "ltag",
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0));
    anything_matched = createEntryBlockAlloca(
        _main, "lanything", llvm::Type::getInt1Ty(TheContext));
    chars_since_last_final = createEntryBlockAlloca(
        _main, "lfinal_cs", llvm::Type::getInt32Ty(TheContext));
    last_final_state_position = createEntryBlockAlloca(
        _main, "lfinals_p",
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0));
    nlex_errc = createEntryBlockAlloca(_main, "lerrc",
                                       llvm::Type::getInt8Ty(TheContext));
    llvm::IRBuilder<> builder(TheContext);
    builder.SetInsertPoint(main_entry);
    builder.CreateStore(
        llvm::ConstantInt::get(chars_since_last_final->getAllocatedType(), 0),
        chars_since_last_final);
    builder.CreateStore(builder.CreateCall(nlex_current_p, {}),
                        last_final_state_position);
    builder.CreateStore(
        llvm::ConstantInt::getFalse(llvm::Type::getInt1Ty(TheContext)),
        anything_matched);
    if (clear) {
      builder.CreateStore(
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0),
          token_length);
      builder.CreateStore(
          llvm::ConstantInt::get(llvm::Type::getInt8Ty(TheContext), 0),
          nlex_errc);
      builder.CreateStore(builder.CreateCall(nlex_current_p, {}),
                          nlex_match_start);
      auto isstopwordv = builder.CreateInBoundsGEP(
          _main->arg_begin(),
          {llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 0),
           // metadata
           llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), 4)});
      builder.CreateStore(
          llvm::ConstantInt::get(llvm::Type::getInt8Ty(TheContext), 0),
          isstopwordv);
    }
    return _main;
  }

  llvm::Function *_cmain;
  llvm::Function *current_main() const { return _cmain; }
  llvm::Function *main() {
    if (_main)
      return _main;
    llvm::Type *members[] = {
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext),
                               0),          // token value
        llvm::Type::getInt32Ty(TheContext), // token length
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext),
                               0),         // token tag
        llvm::Type::getInt8Ty(TheContext), // error code (0 = ok)
        llvm::Type::getInt8Ty(TheContext), // metadata (1 = stopword, )
    };
    input_struct_type = llvm::StructType::create(members);
    _main = mkfunc();
    main_entry = &_main->getEntryBlock();
    _main->setSubprogram(enterSubprogram(_main));
    start = main_entry;
    return _main;
  }
  auto createGlobal(llvm::Type *type, llvm::Constant *value, std::string name,
                    bool dexport = false) {
    auto res =
        new llvm::GlobalVariable(type, false,
                                 dexport ? llvm::GlobalValue::ExternalLinkage
                                         : llvm::GlobalValue::InternalLinkage,
                                 value, name);
    TheModule->getGlobalList().push_back(res);
    return res;
  }
  Module(std::string name, llvm::raw_ostream *os)
      : TheContext(), Builder(TheContext), outputv(os) {
    TheModule = std::make_unique<llvm::Module>(name, TheContext);
    TheModule->addModuleFlag(llvm::Module::Warning, "Dwarf Version",
                             llvm::dwarf::DWARF_VERSION);
    TheModule->addModuleFlag(llvm::Module::Warning, "Debug Info Version",
                             llvm::DEBUG_METADATA_VERSION);

    DBuilder = new llvm::DIBuilder(*TheModule.get());
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    DIFile = DBuilder->createFile(name, buf);
    TheCU = DBuilder->createCompileUnit(llvm::dwarf::DW_LANG_C, DIFile,
                                        "NLex Compiler", false, "", 0);

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

    nlex_skip = llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), {}, false),
        llvm::Function::ExternalLinkage, "__nlex_skip", TheModule.get());

    llvm::FunctionType *nc =
        llvm::FunctionType::get(llvm::Type::getInt64Ty(TheContext), {}, false);

    nlex_distance = llvm::Function::Create(nc, llvm::Function::ExternalLinkage,
                                           "__nlex_distance", TheModule.get());
    nlex_match_start = createGlobal(
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0),
        llvm::Constant::getNullValue(
            llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0)),
        "nlex_match_start");

    token_value = createGlobal(
        llvm::ArrayType::get(llvm::Type::getInt8Ty(TheContext), 1024000),
        llvm::Constant::getNullValue(
            llvm::ArrayType::get(llvm::Type::getInt8Ty(TheContext), 1024000)),
        "ltoken_value");

    token_length = createGlobal(
        llvm::Type::getInt32Ty(TheContext),
        llvm::Constant::getNullValue(llvm::Type::getInt32Ty(TheContext)),
        "ltoken_length");
  }
};

class Builder {
public:
  Module module;
  llvm::raw_ostream *outputv = nullptr;
  llvm::BasicBlock *first_root = nullptr;
  bool issubexp = false;

  Builder(std::string mname, llvm::raw_ostream *o)
      : outputv(o), module(mname, o) {}

  void begin(llvm::Function *fn, bool cleanup_if_fail = false,
             bool skip_on_error = true) {
    module.emitLocation((DFANode<NFANode<std::nullptr_t> *> *)NULL);
    auto BBfinalise =
        llvm::BasicBlock::Create(module.TheContext, "_escape_top", fn);
    module.Builder.SetInsertPoint(BBfinalise);
    if (!module.BBfinalise)
      module.BBfinalise = BBfinalise;
    auto bbF = llvm::BasicBlock::Create(module.TheContext, "_escape", fn);
    if (skip_on_error) {
      auto advance_and_callFbb =
          llvm::BasicBlock::Create(module.TheContext, "_escape_redo", fn);
      module.Builder.CreateCondBr(
          module.Builder.CreateLoad(module.anything_matched), bbF,
          advance_and_callFbb);
      module.Builder.SetInsertPoint(advance_and_callFbb);
      module.Builder.CreateCall(module.nlex_next);
      module.Builder.CreateCall(fn, {fn->arg_begin()})->setTailCall(true);
      module.Builder.CreateRetVoid();
    } else
      module.Builder.CreateBr(bbF);
    module.Builder.SetInsertPoint(bbF);
    auto istruct = fn->arg_begin();
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
    auto errc = module.Builder.CreateLoad(module.nlex_errc);
    module.Builder.CreateStore(
        errc, module.Builder.CreateInBoundsGEP(
                  istruct,
                  {
                      llvm::ConstantInt::get(
                          llvm::Type::getInt32Ty(module.TheContext), 0),
                      llvm::ConstantInt::get(
                          llvm::Type::getInt32Ty(module.TheContext), 3),
                  },
                  "errc"));
    if (cleanup_if_fail) {
      auto mbb = llvm::BasicBlock::Create(module.TheContext, "_cleanup", fn);
      auto ebb = llvm::BasicBlock::Create(module.TheContext, "_exit", fn);
      module.Builder.CreateCondBr(
          module.Builder.CreateICmpEQ(
              errc, llvm::ConstantInt::get(
                        llvm::Type::getInt8Ty(module.TheContext), 0)),
          ebb, mbb);
      module.Builder.SetInsertPoint(mbb);
      auto nfs = module.nlex_fed_string;
      module.Builder.CreateStore(
          module.Builder.CreateInBoundsGEP(
              module.Builder.CreateLoad(nfs),
              {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                      -1)}),
          nfs);
      module.Builder.CreateBr(ebb);
      module.Builder.SetInsertPoint(ebb);
    }
    module.Builder.CreateRetVoid();
  }
  void prepare(const GenLexer &&lexer_stuff) {
    // create global values & normalisation logic
    module.emitLocation((DFANode<NFANode<std::nullptr_t> *> *)NULL);
    {
      auto nlex_fed_string = module.nlex_fed_string = module.createGlobal(
          llvm::PointerType::get(llvm::Type::getInt8Ty(module.TheContext), 0),
          llvm::Constant::getNullValue(llvm::PointerType::get(
              llvm::Type::getInt8Ty(module.TheContext), 0)),
          "nlex_fed_string");
      auto nlex_true_start = module.createGlobal(
          llvm::PointerType::get(llvm::Type::getInt8Ty(module.TheContext), 0),
          llvm::Constant::getNullValue(llvm::PointerType::get(
              llvm::Type::getInt8Ty(module.TheContext), 0)),
          "nlex_true_start");
      auto nlex_tmp_char =
          module.createGlobal(llvm::Type::getInt8Ty(module.TheContext),
                              llvm::Constant::getNullValue(
                                  llvm::Type::getInt8Ty(module.TheContext)),
                              "nlex_tmp_char");
      auto nlex_injected = module.createGlobal(
          llvm::PointerType::get(llvm::Type::getInt8Ty(module.TheContext), 0),
          llvm::Constant::getNullValue(llvm::PointerType::get(
              llvm::Type::getInt8Ty(module.TheContext), 0)),
          "nlex_injected");
      auto nlex_injected_length =
          module.createGlobal(llvm::Type::getInt32Ty(module.TheContext),
                              llvm::Constant::getNullValue(
                                  llvm::Type::getInt8Ty(module.TheContext)),
                              "nlex_injected_length");
      auto nlex_injected_length_diff =
          module.createGlobal(llvm::Type::getInt32Ty(module.TheContext),
                              llvm::Constant::getNullValue(
                                  llvm::Type::getInt8Ty(module.TheContext)),
                              "nlex_injected_length_diff");

      // create "library" functions
      llvm::IRBuilder<> builder(module.TheContext);

      // nlex_current_f - get current character
      auto BB = llvm::BasicBlock::Create(module.TheContext, "",
                                         module.nlex_current_f);
      builder.SetInsertPoint(BB);
      builder.CreateRet(builder.CreateLoad(nlex_tmp_char));

      // nlex_current_p - get current position in string
      BB = llvm::BasicBlock::Create(module.TheContext, "",
                                    module.nlex_current_p);
      builder.SetInsertPoint(BB);
      builder.CreateRet(
          builder.CreateGEP(builder.CreateLoad(nlex_fed_string),
                            builder.CreateLoad(nlex_injected_length_diff)));

      // nlex_distance - get current position in string (as int)
      BB =
          llvm::BasicBlock::Create(module.TheContext, "", module.nlex_distance);
      builder.SetInsertPoint(BB);
      builder.CreateRet(builder.CreatePtrDiff(
          builder.CreateGEP(builder.CreateLoad(nlex_fed_string),
                            builder.CreateLoad(nlex_injected_length_diff)),
          builder.CreateLoad(nlex_true_start)));

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
          builder.CreateNSWSub(
              len, llvm::ConstantInt::get(
                       llvm::Type::getInt32Ty(module.TheContext), 1)),
          nlex_injected_length);
      auto ld = builder.CreateLoad(nlex_injected);
      builder.CreateStore(
          builder.CreateGEP(ld,
                            llvm::ConstantInt::get(
                                llvm::Type::getInt32Ty(module.TheContext), 1)),
          nlex_injected);
      builder.CreateStore(builder.CreateLoad(ld), nlex_tmp_char);
      builder.CreateRetVoid();
      // no inject
      BB = pBB;
      builder.SetInsertPoint(BB);
      auto fs = builder.CreateLoad(nlex_fed_string);
      auto gep = builder.CreateInBoundsGEP(
          fs, {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                      1)});
      builder.CreateStore(gep, nlex_fed_string);
      auto cvv = builder.CreateLoad(fs);
      // create a select of all specified normalisations and then set
      ::std::map<std::string, llvm::SwitchInst *> levels;
      llvm::IRBuilder<> mbuilder(module.TheContext);
      auto BBend = llvm::BasicBlock::Create(module.TheContext, "default_escape",
                                            module.nlex_next);
      // create normalisation logic
      {
        if (lexer_stuff.normalisations.size() == 0)
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
        for (auto pnorm : lexer_stuff.normalisations) {
          mbuilder.SetInsertPoint(BB);
          auto norm = pnorm.first;
          slts.show_c(Display::Type::DEBUG,
                      "[NORMALISE] {<green>}%d (%s) -> %d (%s){<clean>}",
                      norm.size(), norm, pnorm.second.size(), pnorm.second);
          for (int i = 0; i <= norm.size(); i++) {
            if (!levels.count(norm.substr(0, i))) {
              // we don't have it in the cache
              // create a toplevel block
              llvm::SwitchInst *sw;
              if (i == norm.size()) {
                if (i > 1)
                  mbuilder.CreateStore(
                      mbuilder.CreateGEP(
                          mbuilder.CreateLoad(nlex_fed_string),
                          llvm::ConstantInt::get(
                              llvm::Type::getInt32Ty(module.TheContext),
                              norm.size() - 1)),
                      nlex_fed_string);
                mbuilder.CreateStore(
                    get_or_create_tag(
                        pnorm.second.substr(1, pnorm.second.size()), false),
                    nlex_injected); // consume one byte and store the rest
                mbuilder.CreateStore(
                    llvm::ConstantInt::get(
                        llvm::Type::getInt32Ty(module.TheContext),
                        pnorm.second.size() - 1),
                    nlex_injected_length);
                mbuilder.CreateStore(
                    llvm::ConstantInt::get(
                        llvm::Type::getInt32Ty(module.TheContext),
                        pnorm.second.size() - norm.size()),
                    nlex_injected_length_diff);

                mbuilder.CreateStore( // store the first character (literal)
                    llvm::ConstantInt::get(
                        llvm::Type::getInt8Ty(module.TheContext),
                        (int)pnorm.second[0]),
                    nlex_tmp_char);
                mbuilder.CreateRetVoid();
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
                  llvm::ConstantInt::get(
                      llvm::Type::getInt8Ty(module.TheContext), (int)norm[i]),
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
                cBB = llvm::BasicBlock::Create(
                    module.TheContext, norm.substr(0, i), module.nlex_next);
                sw->addCase(val, cBB);
              } else
                cBB = sw->getSuccessor(ci->getSuccessorIndex());
              mbuilder.SetInsertPoint(cBB);
            }
          }
        }
      }

      // nlex_start - return the true start of the fed string
      BB = llvm::BasicBlock::Create(module.TheContext, "", module.nlex_start);
      builder.SetInsertPoint(BB);
      builder.CreateRet(builder.CreateLoad(nlex_true_start));

      // nlex_skip - skip a char
      BB = llvm::BasicBlock::Create(module.TheContext, "", module.nlex_skip);
      builder.SetInsertPoint(BB);
      builder.CreateCall(module.nlex_next);
      builder.CreateRetVoid();

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
          arg, llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext),
                                      127));
      builder.CreateCondBr(icmp, _11, _4);
      builder.SetInsertPoint(_4);
      auto icmp_4 = builder.CreateICmpULT(
          arg, llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext),
                                      -33));
      builder.CreateCondBr(icmp_4, _11, _6);
      builder.SetInsertPoint(_6);
      auto icmp_6 = builder.CreateICmpULT(
          arg, llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext),
                                      -17));
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
      auto phi =
          builder.CreatePHI(llvm::Type::getInt32Ty(module.TheContext), 4);
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
    }
    // Create a stopword remover if any stopwords are present
    if (lexer_stuff.stopwords.size() > 0) {
      auto fbb = llvm::BasicBlock::Create(module.TheContext, "_stopword_res",
                                          module.main());
      auto *prev_fbb = module.BBfinalise;
      WordTree<std::string> wtree{lexer_stuff.stopwords};
      llvm::IRBuilder<> builder{module.TheContext};
      module.emitLocation((DFANode<NFANode<std::nullptr_t> *> *)NULL, builder);

      auto tcabb = llvm::BasicBlock::Create(module.TheContext, "_exit_stopword",
                                            module.main());
      builder.SetInsertPoint(tcabb);
      if (get(lexer_stuff.options, "ignore_stopwords")) {
        builder.CreateCall(module.main(), {module.main()->arg_begin()})
            ->setTailCall(true);
        builder.CreateRetVoid();
      } else {
        // just set a tag and return with it
        auto isstopwordv = builder.CreateInBoundsGEP(
            module.main()->arg_begin(),
            {
                llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module.TheContext), 0),
                llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module.TheContext),
                    4) // metadata
            });
        builder.CreateStore(
            builder.CreateOr(llvm::ConstantInt::get(
                                 llvm::Type::getInt8Ty(module.TheContext), 1),
                             builder.CreateLoad(isstopwordv)),
            isstopwordv);
        builder.CreateBr(prev_fbb);
      }
      builder.SetInsertPoint(fbb);
      builder.CreateStore(
          llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.TheContext), 0),
          builder.CreateInBoundsGEP(
              module.token_value,
              {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                      0),
               builder.CreateLoad(module.token_length)}));

      std::queue<std::tuple<decltype(wtree.root_node), llvm::BasicBlock *, int>>
          qnodes;
      qnodes.push({wtree.root_node, fbb, 0});

      while (!qnodes.empty()) {
        auto [nodes, bb, i] = qnodes.front();
        qnodes.pop();
        auto nfbb =
            llvm::BasicBlock::Create(module.TheContext, "", module.main());

        builder.SetInsertPoint(bb);
        builder.CreateCondBr(
            builder.CreateICmpSLT(
                builder.CreateLoad(module.token_length),
                llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(module.TheContext), i)),
            prev_fbb, nfbb);

        builder.SetInsertPoint(nfbb);
        auto cc = builder.CreateLoad(builder.CreateInBoundsGEP(
            module.token_value,
            {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                    0),
             llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                    i)}));
        auto sw = builder.CreateSwitch(cc, prev_fbb);

        for (auto [c, node] : *nodes) {
          // oh boy
          // let's hope that char{} is actually 0
          slts.show(Display::Type::DEBUG,
                    "[{<red>}Stopword{<clean>}] [{<red>}Resolution{<clean>}] "
                    "char {<magenta>}%c{<clean>}",
                    c);
          if (c == 0)
            sw->addCase(llvm::ConstantInt::get(
                            llvm::Type::getInt8Ty(module.TheContext), 0),
                        tcabb); // jump back to main
          else {
            auto nodefb = llvm::BasicBlock::Create(
                module.TheContext, "_stopword_res_" + std::string{c},
                module.main());
            sw->addCase(llvm::ConstantInt::get(
                            llvm::Type::getInt8Ty(module.TheContext), (int)c),
                        nodefb);
            qnodes.push({node, nodefb, i + 1});
          }
        }
      }
      module.BBfinalise = fbb;
    }
    // skip ignored states if specified
    if (lexer_stuff.ignores.size() > 0) {
      auto fbb = llvm::BasicBlock::Create(module.TheContext, "_ignore_res",
                                          module.main());
      auto *prev_fbb = module.BBfinalise;
      WordTree<std::string> wtree{lexer_stuff.stopwords};
      llvm::IRBuilder<> builder{module.TheContext};
      module.emitLocation((DFANode<NFANode<std::nullptr_t> *> *)NULL, builder);

      auto tcabb = llvm::BasicBlock::Create(module.TheContext, "_tailcall_self",
                                            module.main());
      builder.SetInsertPoint(tcabb);
      builder.CreateCall(module.main(), {module.main()->arg_begin()})
          ->setTailCall(true);
      builder.CreateRetVoid();

      builder.SetInsertPoint(fbb);
      auto tag = builder.CreateLoad(module.last_tag);
      auto ptag = builder.CreatePtrToInt(
          tag, llvm::Type::getInt64Ty(module.TheContext));
      llvm::BasicBlock *nextbb;
      for (auto ign : lexer_stuff.ignores) {
        nextbb = llvm::BasicBlock::Create(module.TheContext,
                                          "__ignore_res_" + ign, module.main());
        builder.CreateCondBr(
            builder.CreateICmpEQ(get_or_create_tag_constint(ign), ptag), tcabb,
            nextbb);
        builder.SetInsertPoint(nextbb);
      }
      builder.CreateBr(prev_fbb);
      module.BBfinalise = fbb;
    }
    // add a function that just reads the input and writes the normalised form
    // out
    if (get(lexer_stuff.options, "pure_normaliser")) {
      llvm::Function *pure_normalise;
      llvm::FunctionType *ncf = llvm::FunctionType::get(
          llvm::Type::getInt8Ty(module.TheContext), {}, false);

      pure_normalise = llvm::Function::Create(
          ncf, llvm::Function::ExternalLinkage, "__nlex_pure_normalise",
          module.TheModule.get());
      auto entry =
          llvm::BasicBlock::Create(module.TheContext, "", pure_normalise);
      llvm::IRBuilder<> fbuilder{module.TheContext};
      fbuilder.SetInsertPoint(entry);
      fbuilder.CreateCall(module.nlex_next);
      fbuilder.CreateRet(fbuilder.CreateCall(module.nlex_current_f));
    }
    // handle all literally tagged values
    if (lexer_stuff.literal_tags.size() > 0) {
      llvm::IRBuilder<> builder(module.TheContext);
      builder.SetInsertPoint(&module.main()->getEntryBlock());
      llvm::BasicBlock *start =
          llvm::BasicBlock::Create(module.TheContext, "start", module.main());
      module.start = start;
      llvm::BasicBlock *check = llvm::BasicBlock::Create(
          module.TheContext, "__literal", module.main());
      builder.CreateBr(check);
      builder.SetInsertPoint(check);

      for (auto [tag, values] : lexer_stuff.literal_tags) {
        WordTree<std::string, std::string, std::vector<std::string>> wt{
            values, WordTreeActions::store_value_tag{}};
        std::queue<std::tuple<decltype(wt.root_node), llvm::BasicBlock *, int>>
            queue;
        queue.push({wt.root_node, check, 0});
        auto strv = builder.CreateLoad(module.nlex_fed_string);
        while (!queue.empty()) {
          auto [nodes, nodebb, offsetidx] = queue.front();
          queue.pop();
          builder.SetInsertPoint(nodebb);
          auto swinst = builder.CreateSwitch(
              builder.CreateLoad(builder.CreateInBoundsGEP(
                  strv,
                  {llvm::ConstantInt::get(
                      llvm::Type::getInt32Ty(module.TheContext), offsetidx)})),
              start, nodes->elements.size());
          for (auto [c, node] : *nodes) {
            // EOW - tag matches, emit new tag and return
            if (c == 0) {
              builder.SetInsertPoint(swinst);
              // reset token length (write from the beginning of the token
              // value)
              builder.CreateStore(
                  llvm::ConstantInt::get(
                      llvm::Type::getInt32Ty(module.TheContext), 0),
                  module.token_length);
              // set token value
              for (auto c : node->metadata)
                module.add_char_to_token(c, builder);

              // set tag
              builder.CreateStore(get_or_create_tag(tag), module.last_tag);
              // set matched flag
              builder.CreateStore(llvm::ConstantInt::getTrue(
                                      llvm::Type::getInt1Ty(module.TheContext)),
                                  module.anything_matched);
              // increment nlex_fed
              builder.CreateCall(
                  module.nlex_restore,
                  {builder.CreateInBoundsGEP(
                      builder.CreateCall(module.nlex_current_p),
                      {llvm::ConstantInt::get(
                          llvm::Type::getInt32Ty(module.TheContext),
                          offsetidx)})});
              builder.CreateStore(builder.CreateCall(module.nlex_current_p, {}),
                                  module.last_final_state_position);
              builder.CreateStore(
                  llvm::ConstantInt::get(
                      llvm::Type::getInt32Ty(module.TheContext), 0),
                  module.chars_since_last_final);
              builder.CreateStore(
                  llvm::ConstantInt::get(
                      llvm::Type::getInt8Ty(module.TheContext), 0),
                  module.nlex_errc);
              // jump to end
              swinst->setDefaultDest(module.BBfinalise);
              continue;
            }

            // generate case and jump to it
            llvm::BasicBlock *cbb = llvm::BasicBlock::Create(
                module.TheContext,
                (("__literal_" + tag) + "_") + std::string{c}, module.main());
            swinst->addCase(llvm::ConstantInt::get(
                                llvm::Type::getInt8Ty(module.TheContext), c),
                            cbb);
            queue.push({node, cbb, offsetidx + 1});
          }
          // swap empty switch with jump
          if (swinst->getNumCases() == 0) {
            builder.SetInsertPoint(swinst);
            builder.CreateBr(swinst->getDefaultDest());
            swinst->removeFromParent();
          }
        }
      }
    }
    // likely to be replaced at link-time with a separate module
    /* if constexpr (false) */
    {
      // generate a pos tagger if we have it specified
      module.createGlobal(llvm::Type::getInt1Ty(module.TheContext),
                          (lexer_stuff.tagpos.has_value()
                               ? llvm::ConstantInt::getTrue(
                                     llvm::Type::getInt1Ty(module.TheContext))
                               : llvm::ConstantInt::getFalse(
                                     llvm::Type::getInt1Ty(module.TheContext))),
                          "__nlex_has_tagpos", true);
      module.createGlobal(
          llvm::PointerType::get(llvm::Type::getInt8Ty(module.TheContext), 0),
          (llvm::Constant
               *)(lexer_stuff.tagpos.has_value()
                      ? get_or_create_tag(lexer_stuff.tagpos.value().rule)
                      : llvm::Constant::getNullValue(llvm::PointerType::get(
                            llvm::Type::getInt8Ty(module.TheContext), 0))),
          "__nlex_tagpos");
      get_or_create_tag(lexer_stuff.tagpos->rule, true, "__nlex_ptag");
      if (lexer_stuff.tagpos.has_value()) {
        auto gentag =
            module.mkfunc(false, "__nlex_generated_postag", false, true);
        // generate some magic pos tagger from the model
        mk_string(module.TheModule.get(), module.TheContext,
                  lexer_stuff.tagpos->from, "__nlex_tagpos_filename");
        module.createGlobal(
            llvm::Type::getInt32Ty(module.TheContext),
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                   lexer_stuff.tagpos->gram),
            "__nlex_tagpos_gram", true);
        llvm::IRBuilder<> builder{module.TheContext};
        builder.SetInsertPoint(&gentag->getEntryBlock());
        builder.CreateRetVoid();
      }
    }
  }
  void end() {
    // finish the function
    module.DBuilder->finalize();
    llvm::verifyFunction(*module.main());
    module.TheModule->print(*outputv, nullptr);
  }

  llvm::Constant *mk_string(llvm::Module *M, llvm::LLVMContext &Context,
                            const std::string &svalue,
                            const std::string name = "str") {
    llvm::ArrayType *Ty =
        llvm::ArrayType::get(llvm::Type::getInt8Ty(Context), svalue.size() + 1);
    llvm::GlobalVariable *GV = new llvm::GlobalVariable(
        *M, Ty, true,
        name == "str" ? llvm::GlobalValue::InternalLinkage
                      : llvm::GlobalValue::ExternalLinkage,
        llvm::ConstantDataArray::getString(Context, svalue), name);

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
  llvm::Value *get_or_create_tag(std::string tag, bool istag = true,
                                 std::string name = "str") {
    if (name != "str") {
      return mk_string(module.TheModule.get(), module.TheContext, tag, name);
    }
    if (istag) {
      auto pos = tag.find("{::}");
      if (pos != tag.npos)
        tag = tag.substr(0, pos);
    }
    if (registered_tags.count(tag))
      return registered_tags[tag];
    return registered_tags[tag] =
               mk_string(module.TheModule.get(), module.TheContext, tag);
  }
  llvm::ConstantInt *get_or_create_tag_constint(std::string tag) {
    llvm::Constant *s;
    if (registered_tags.count(tag))
      s = registered_tags[tag];
    else
      s = registered_tags[tag] =
          mk_string(module.TheModule.get(), module.TheContext, tag);
    return to_int(s);
  }
  llvm::ConstantInt *to_int(llvm::Constant *s) {
    return (llvm::ConstantInt *)llvm::ConstantExpr::getPtrToInt(
        s, llvm::Type::getInt64Ty(module.TheContext));
  }
}; // namespace nlvm
} // namespace nlvm
