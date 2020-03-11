#pragma once

#include "basevm.hpp"
#include "genlexer.hpp"
#include "hmm.hpp"
#include "target_triple.hpp"
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
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Linker/Linker.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <memory>
#include <queue>
#include <set>
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
extern std::string output_file_name;
extern nlvm::TargetTriple targetTriple;

namespace nlvm {
static llvm::AllocaInst *createEntryBlockAlloca(llvm::Function *TheFunction,
                                                const std::string &VarName,
                                                llvm::Type *ty,
                                                bool init = false) {
  llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                         TheFunction->getEntryBlock().begin());
  auto alloca = TmpB.CreateAlloca(ty, 0, VarName.c_str());
  if (init)
    TmpB.CreateStore(llvm::Constant::getNullValue(ty), alloca);
  return alloca;
}
struct Module : public nlvm::BaseModule {
public:
  llvm::IRBuilder<> Builder;

  llvm::BasicBlock *start;

  std::vector<llvm::BasicBlock *> exit_blocks = {};

  llvm::AllocaInst *getFreshBranchAlloca(llvm::BasicBlock *bb) {
    auto &alloca = block_allocas[bb];
    if (alloca == nullptr)
      return block_allocas[bb] = createEntryBlockAlloca(
                 main(), "lbtr_flag", llvm::Type::getInt1Ty(TheContext), true);
  }

  llvm::DIBuilder *DBuilder;
  llvm::DICompileUnit *TheCU;
  llvm::DIFile *DIFile;
  std::vector<llvm::DIScope *> LexicalDebugBlocks;

  std::map<llvm::BasicBlock *, llvm::AllocaInst *> block_allocas = {};

  llvm::Function *nlex_current_f;
  llvm::Function *nlex_current_p;
  llvm::Function *nlex_distance;
  llvm::Function *nlex_restore;
  llvm::Function *nlex_feed;
  llvm::Function *nlex_next;
  llvm::Function *nlex_start;
  llvm::Function *nlex_get_utf8_length;
  llvm::Function *nlex_skip;
  llvm::Function *nlex_debug;

  llvm::Function *nlex_get_group_start_ptr = nullptr;
  llvm::Function *nlex_get_group_end_ptr = nullptr;
  llvm::Function *nlex_get_group_length = nullptr;

  llvm::Function *nlex_apply_postag = nullptr;
  bool postag_applies = false;

  /// Stores the start of this match
  llvm::GlobalVariable *nlex_match_start;
  /// Stores the value of the proceeding token
  llvm::GlobalVariable *token_value;
  /// Stores the valid length of token_value
  llvm::GlobalVariable *token_length;
  /// Stores the subject string
  llvm::GlobalVariable *nlex_fed_string;
  /// Stores the capture indices
  /// [i0_start, i0_end, i1_start, i1_end] (start = i*2, end = i*2+1)
  /// exists only if `option capture_groups on`
  llvm::GlobalVariable *nlex_capture_indices = nullptr;

  llvm::Function *_main = nullptr;
  llvm::BasicBlock *main_entry, *BBfinalise = nullptr, *backtrackBB = nullptr,
                                *backtrackExitBB = nullptr;

  llvm::AllocaInst *last_tag;
  llvm::AllocaInst *last_final_state_position;
  llvm::AllocaInst *chars_since_last_final;
  llvm::AllocaInst *anything_matched;
  llvm::AllocaInst *nlex_errc;
  llvm::AllocaInst *last_backtrack_branch_position;

  llvm::Type *input_struct_type;

  llvm::raw_ostream *outputv;

  bool debug_mode = false;

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
  void emitLocation(debug_offset_info offset_info, llvm::IRBuilder<> &builder) {
    llvm::DIScope *Scope;
    if (LexicalDebugBlocks.empty())
      Scope = TheCU;
    else
      Scope = LexicalDebugBlocks.back();
    builder.SetCurrentDebugLocation(
        llvm::DebugLoc::get(offset_info.line, offset_info.offset, Scope));
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
                         bool toplevels = true, bool nullary = false,
                         bool nodefine = false,
                         llvm::FunctionType *fnty = nullptr) {
    llvm::Function *_main;
    if (fnty == nullptr) {
      llvm::ArrayRef<llvm::Type *> args = {
          llvm::PointerType::get(input_struct_type, 0)};
      llvm::ArrayRef<llvm::Type *> nnargs = {};
      llvm::FunctionType *ncf = llvm::FunctionType::get(
          llvm::Type::getVoidTy(TheContext), nullary ? nnargs : args, false);

      _main = llvm::Function::Create(
          ncf, llvm::Function::LinkageTypes::ExternalLinkage, name,
          TheModule.get());
    } else
      _main = llvm::Function::Create(
          fnty, llvm::Function::LinkageTypes::ExternalLinkage, name,
          TheModule.get());

    if (nodefine)
      return _main;
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
    last_backtrack_branch_position = createEntryBlockAlloca(
        _main, "lbtr_p",
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0));
    nlex_errc = createEntryBlockAlloca(_main, "lerrc",
                                       llvm::Type::getInt8Ty(TheContext));
    llvm::IRBuilder<> builder(TheContext);
    builder.SetInsertPoint(main_entry);
    builder.CreateStore(
        llvm::ConstantInt::get(chars_since_last_final->getAllocatedType(), 0),
        chars_since_last_final);
    auto val = builder.CreateCall(nlex_current_p, {});
    builder.CreateStore(val, last_final_state_position);
    // store backtrack position
    builder.CreateStore(val, last_backtrack_branch_position);
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
        llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0), // POS
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

  std::unique_ptr<llvm::Module> RTSModule;
  std::unique_ptr<llvm::Module> DeserModule;

  Module(std::string name, llvm::raw_ostream *os)
      : BaseModule(), Builder(TheContext), outputv(os) {
    llvm::SMDiagnostic ed;

// #define unsigned const
#include "test_bc.h"
    // #undef unsigned

    static llvm::StringRef mLibraryBitcode =
        llvm::StringRef((const char *)test_bc, test_bc_len);
    RTSModule = llvm::parseIR(llvm::MemoryBufferRef(mLibraryBitcode, "test_bc"),
                              ed, TheContext);

    if (!RTSModule)
      ed.print(name.c_str(), *os);
    assert(RTSModule.get() != nullptr && "RTS compilation failed");

#include "deser.inc"
    static llvm::StringRef mDeserBitcode =
        llvm::StringRef((const char *)deser_inc_bc, deser_inc_bc_len);
    DeserModule = llvm::parseIR(
        llvm::MemoryBufferRef(mDeserBitcode, "deser_bc"), ed, TheContext);

    if (!DeserModule)
      ed.print(name.c_str(), *os);
    assert(DeserModule.get() != nullptr && "Deser compilation failed");

    TheModule = std::make_unique<llvm::Module>(name, TheContext);

    TheModule->addModuleFlag(llvm::Module::Warning, "Dwarf Version",
                             llvm::dwarf::DWARF_VERSION);
    TheModule->addModuleFlag(llvm::Module::Warning, "Debug Info Version",
                             llvm::DEBUG_METADATA_VERSION);

    TheMPM = std::make_unique<llvm::legacy::PassManager>();

    TheMPM->add(llvm::createInstructionCombiningPass());
    TheMPM->add(llvm::createReassociatePass());
    TheMPM->add(llvm::createGVNPass());
    // TheMPM->add(llvm::createCFGSimplificationPass());
    TheMPM->add(llvm::createLICMPass());
    TheMPM->add(llvm::createAggressiveDCEPass());
    TheMPM->add(llvm::createConstantPropagationPass());
    // TheMPM->add(llvm::createTailCallEliminationPass());
    TheMPM->add(llvm::createInstructionCombiningPass());
    // TheMPM->add(llvm::createSinkingPass());
    // TheMPM->add(llvm::createCFGSimplificationPass());

    // TheMAM = std::make_unique<llvm::ModuleAnalysisManager>();

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
        llvm::Function::Create(ucf, llvm::Function::ExternalLinkage,
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
} // namespace nlvm

extern void KaleidInitialise(std::string startup_code,
                             nlvm::BaseModule *module);
extern void KaleidCompile(std::string code, llvm::IRBuilder<> &builder,
                          bool allow_bare_expressions);

namespace nlvm {
static void debugPrintValue(llvm::Value *v) { v->print(llvm::errs(), true); }
static void debugPrintType(llvm::Value *v) {
  v->getType()->print(llvm::errs(), true);
}

class Builder {
public:
  Module module;
  llvm::raw_ostream *outputv = nullptr;
  llvm::BasicBlock *first_root = nullptr;
  bool issubexp = false;
  bool do_capture_groups = false;
  llvm::TargetMachine *TheTargetMachine;

  Builder(std::string mname, llvm::raw_ostream *o)
      : outputv(o), module(mname, o) {
    using namespace llvm;
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();

    auto TargetTriple = sys::getDefaultTargetTriple();
    if (targetTriple._cross)
      TargetTriple = targetTriple.triple.str();
    module.TheModule->setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);
    if (!Target) {
      slts.show(Display::Type::ERROR,
                "Invalid or unsupported compilation target "
                "'%s':\n\t{<red>}%s{<clean>}",
                TargetTriple.c_str(), Error.c_str());
      exit(1);
    }
    auto CPU = targetTriple.cpu;
    auto Features = targetTriple.features;

    llvm::TargetOptions opt;
    TheTargetMachine = Target->createTargetMachine(
        TargetTriple, CPU, Features, opt, targetTriple.reloc_model);

    module.TheModule->setDataLayout(TheTargetMachine->createDataLayout());
    module.TheModule->setTargetTriple(TargetTriple);
  }

  void begin(llvm::Function *fn, bool cleanup_if_fail = false,
             bool skip_on_error = true) {
    KaleidInitialise(R"(
        extern putchard(x);
        extern eputchard(x);
        extern printd(x);
        extern eprintd(x);
        extern mallocd(sze);
        extern memsetd(ptr val sze);
        extern freed(ptr);

        extern variadic dprintdf(fd fmt);
        
        extern cderef(ptr);
        extern cderefset(ptr val);
        extern dderef(ptr);
        extern dderefset(ptr val);

        def binary : (x y) y;
        def unary ^ (ptr) cderef(ptr);
        def binary ` (ptr val) cderefset(ptr, val);

        def print_string(str) for x=0,^(str+x) in
            putchard(^(str+x))

        def eprint_string(str) for x=0,^(str+x) in
            eputchard(^(str+x))
    )",
                     &module);

    module.emitLocation((DFANode<NFANode<std::nullptr_t> *> *)NULL);
    auto BBfinalise =
        llvm::BasicBlock::Create(module.TheContext, "_escape_top", fn);
    if (!module.BBfinalise)
      module.BBfinalise = BBfinalise;

    module.Builder.SetInsertPoint(BBfinalise);
    auto bbF = llvm::BasicBlock::Create(module.TheContext, "_escape", fn);

    module.exit_blocks.push_back(bbF);

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
    // produce debug stuff
    if (get(lexer_stuff.options, "debug_mode"))
      module.debug_mode = true;
    // record capture groups if set
    if (get(lexer_stuff.options, "capturing_groups")) {
      do_capture_groups = true;
      auto arrty =
          llvm::ArrayType::get(llvm::Type::getInt8PtrTy(module.TheContext),
                               (lexer_stuff.total_capturing_groups + 1) * 2);
      module.nlex_capture_indices =
          module.createGlobal(arrty, llvm::ConstantArray::getNullValue(arrty),
                              "capturing_group_indices");

      // create lib functions that require this option
      auto iptrt = llvm::FunctionType::get(
          llvm::Type::getInt8PtrTy(module.TheContext),
          {llvm::Type::getDoubleTy(module.TheContext)}, false);
      auto iit = llvm::FunctionType::get(
          llvm::Type::getInt32Ty(module.TheContext),
          {llvm::Type::getDoubleTy(module.TheContext)}, false);
      module.nlex_get_group_start_ptr = llvm::Function::Create(
          iptrt, llvm::GlobalValue::LinkageTypes::InternalLinkage,
          "nlex_get_group_start_ptr", *module.TheModule);
      module.nlex_get_group_end_ptr = llvm::Function::Create(
          iptrt, llvm::GlobalValue::LinkageTypes::InternalLinkage,
          "nlex_get_group_end_ptr", *module.TheModule);
      module.nlex_get_group_length = llvm::Function::Create(
          iit, llvm::GlobalValue::LinkageTypes::InternalLinkage,
          "nlex_get_group_length", *module.TheModule);

      auto total_groups =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                 lexer_stuff.total_capturing_groups);
      llvm::IRBuilder<> builder{module.TheContext};

      // get start ptr
      {
        auto bb0 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_start_ptr);
        auto bb1 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_start_ptr);
        auto bb2 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_start_ptr);

        builder.SetInsertPoint(bb0);
        auto arg =
            builder.CreateFPToSI(module.nlex_get_group_start_ptr->arg_begin(),
                                 llvm::Type::getInt32Ty(module.TheContext));
        builder.CreateCondBr(builder.CreateICmpSGT(arg, total_groups), bb2,
                             bb1);
        builder.SetInsertPoint(bb1);
        auto phiout0 = builder.CreateLoad(builder.CreateInBoundsGEP(
            module.nlex_capture_indices,
            {llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.TheContext),
                                    0),
             builder.CreateSExt(builder.CreateShl(arg,
                                                  llvm::APInt(32, 1, false), "",
                                                  false, true),
                                llvm::Type::getInt64Ty(module.TheContext))}));
        builder.CreateBr(bb2);
        builder.SetInsertPoint(bb2);
        llvm::PHINode *phi =
            builder.CreatePHI(llvm::Type::getInt8PtrTy(module.TheContext), 2);
        phi->addIncoming(llvm::Constant::getNullValue(
                             llvm::Type::getInt8PtrTy(module.TheContext)),
                         bb0);
        phi->addIncoming(phiout0, bb1);
        builder.CreateRet(phi);
      }

      // get end ptr
      {
        auto bb0 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_end_ptr);
        auto bb1 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_end_ptr);
        auto bb2 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_end_ptr);

        builder.SetInsertPoint(bb0);
        auto arg =
            builder.CreateFPToSI(module.nlex_get_group_end_ptr->arg_begin(),
                                 llvm::Type::getInt32Ty(module.TheContext));
        builder.CreateCondBr(builder.CreateICmpSGT(arg, total_groups), bb2,
                             bb1);
        builder.SetInsertPoint(bb1);
        auto phiout0 = builder.CreateLoad(builder.CreateInBoundsGEP(
            module.nlex_capture_indices,
            {llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.TheContext),
                                    0),
             builder.CreateSExt(
                 builder.CreateOr(builder.CreateShl(arg,
                                                    llvm::APInt(32, 1, false),
                                                    "", false, true),
                                  1),
                 llvm::Type::getInt64Ty(module.TheContext))}));
        builder.CreateBr(bb2);
        builder.SetInsertPoint(bb2);
        llvm::PHINode *phi =
            builder.CreatePHI(llvm::Type::getInt8PtrTy(module.TheContext), 2);
        phi->addIncoming(llvm::Constant::getNullValue(
                             llvm::Type::getInt8PtrTy(module.TheContext)),
                         bb0);
        phi->addIncoming(phiout0, bb1);
        builder.CreateRet(phi);
      }

      // get length
      {
        auto bb0 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_length);
        auto bb1 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_length);
        auto bb2 = llvm::BasicBlock::Create(module.TheContext, "",
                                            module.nlex_get_group_length);

        builder.SetInsertPoint(bb0);
        auto arg =
            builder.CreateFPToSI(module.nlex_get_group_length->arg_begin(),
                                 llvm::Type::getInt32Ty(module.TheContext));
        builder.CreateCondBr(builder.CreateICmpSGT(arg, total_groups), bb2,
                             bb1);
        builder.SetInsertPoint(bb1);
        auto a0 = builder.CreateLoad(builder.CreateBitCast(
            builder.CreateInBoundsGEP(
                module.nlex_capture_indices,
                {llvm::ConstantInt::get(
                     llvm::Type::getInt64Ty(module.TheContext), 0),
                 builder.CreateSExt(
                     builder.CreateShl(arg, llvm::APInt(32, 1, false), "",
                                       false, true),
                     llvm::Type::getInt64Ty(module.TheContext))}),
            llvm::Type::getInt64PtrTy(module.TheContext)));
        auto a1 = builder.CreateLoad(builder.CreateBitCast(
            builder.CreateInBoundsGEP(
                module.nlex_capture_indices,
                {llvm::ConstantInt::get(
                     llvm::Type::getInt64Ty(module.TheContext), 0),
                 builder.CreateSExt(
                     builder.CreateOr(
                         builder.CreateShl(arg, llvm::APInt(32, 1, false), "",
                                           false, true),
                         1),
                     llvm::Type::getInt64Ty(module.TheContext))}),
            llvm::Type::getInt64PtrTy(module.TheContext)));
        auto phiout0 =
            builder.CreateTrunc(builder.CreateSub(a1, a0),
                                llvm::Type::getInt32Ty(module.TheContext));
        builder.CreateBr(bb2);
        builder.SetInsertPoint(bb2);
        llvm::PHINode *phi =
            builder.CreatePHI(llvm::Type::getInt32Ty(module.TheContext), 2);
        phi->addIncoming(llvm::Constant::getNullValue(
                             llvm::Type::getInt32Ty(module.TheContext)),
                         bb0);
        phi->addIncoming(phiout0, bb1);
        builder.CreateRet(phi);
      }
    }

    // crucial library functions
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
                                  llvm::Type::getInt32Ty(module.TheContext)),
                              "nlex_injected_length");
      auto nlex_injected_length_diff =
          module.createGlobal(llvm::Type::getInt32Ty(module.TheContext),
                              llvm::Constant::getNullValue(
                                  llvm::Type::getInt32Ty(module.TheContext)),
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

      auto dfnty = llvm::FunctionType::get(
          llvm::Type::getVoidTy(module.TheContext),
          {llvm::Type::getInt32Ty(module.TheContext),
           llvm::PointerType::getInt8PtrTy(module.TheContext, 0),
           llvm::PointerType::getInt8PtrTy(module.TheContext, 0),
           llvm::PointerType::getInt8PtrTy(module.TheContext, 0)},
          false);
      module.nlex_debug = module.mkfunc(true, "__nlex_produce_debug", false,
                                        false, true, dfnty);
    }
    // Create a stopword remover if any stopwords are present
    if (lexer_stuff.stopwords.size() > 0) {
      auto isstopword =
          module.createGlobal(llvm::Type::getInt1Ty(module.TheContext),
                              (llvm::ConstantInt::getFalse(
                                  llvm::Type::getInt1Ty(module.TheContext))),
                              "nlex_is_this_a_stopword", false);
      auto fbb = llvm::BasicBlock::Create(module.TheContext, "_stopword_res",
                                          module.main());
      auto *prev_fbb = module.BBfinalise;
      WordTree<std::string, debug_offset_info> wtree{lexer_stuff.stopwords};
      llvm::IRBuilder<> builder{module.TheContext};
      module.emitLocation(lexer_stuff.stopwords.begin()->second, builder);

      auto tcabb = llvm::BasicBlock::Create(module.TheContext, "_exit_stopword",
                                            module.main());
      builder.SetInsertPoint(tcabb);
      if (get(lexer_stuff.options, "ignore_stopwords")) {
        builder.CreateCall(module.main(), {module.main()->arg_begin()})
            ->setTailCall(true);
        builder.CreateRetVoid();
      } else {
        // just set a tag and return with it
        builder.CreateStore(llvm::ConstantInt::getTrue(
                                llvm::Type::getInt1Ty(module.TheContext)),
                            isstopword);
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
          llvm::ConstantInt::getFalse(llvm::Type::getInt1Ty(module.TheContext)),
          isstopword);
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
      WordTree<std::string, debug_offset_info> wtree{lexer_stuff.stopwords};
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
      for (auto [ign, dbg] : lexer_stuff.ignores) {
        nextbb = llvm::BasicBlock::Create(module.TheContext,
                                          "__ignore_res_" + ign, module.main());
        module.emitLocation(dbg, builder);
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
    // emit any KDefines specified
    if (lexer_stuff.kdefines.size() > 0) {
      llvm::IRBuilder<> builder(module.TheContext);
      for (auto &kdef : lexer_stuff.kdefines) {
        KaleidCompile(kdef, builder, true);
      }
    }
    // likely to be replaced at link-time with a separate module
    // emit pos tagger stuff if that's specified
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
        // auto gentag =
        //     module.mkfunc(false, "__nlex_generated_postag", false, true);
        // generate some magic pos tagger from the model
        mk_string(module.TheModule.get(), module.TheContext,
                  lexer_stuff.tagpos->from, "__nlex_tagpos_filename");
        module.createGlobal(
            llvm::Type::getInt32Ty(module.TheContext),
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                   lexer_stuff.tagpos->gram),
            "__nlex_tagpos_gram", true);
        std::string postag_data = nlex::POSTag::train(lexer_stuff.tagpos->from);
        auto *data = mk_string(module.TheModule.get(), module.TheContext,
                               postag_data, "__nlex_postag_data");
        module.createGlobal(
            llvm::Type::getInt32Ty(module.TheContext),
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                   postag_data.size()),
            "__nlex_postag_data_length", true);

        module.nlex_apply_postag =
            module.mkfunc(true, "__nlex_apply_postag", false, false, true);
        module.postag_applies = true;

        // llvm::IRBuilder<> builder{module.TheContext};
        // builder.SetInsertPoint(&gentag->getEntryBlock());
        // builder.CreateRetVoid();

        // patch exit blocks of __nlex_root to call "__nlex_apply_postag"
        std::vector<llvm::Value *> args;
        for (auto &arg : module.main()->args())
          args.push_back(&arg);

        for (llvm::BasicBlock *block : module.exit_blocks) {
          for (auto &insn : block->getInstList()) {
            if (insn.getOpcode() == llvm::Instruction::Ret) {
              // prepend a call to __nlex_apply_tag to the bb before the return
              // instruction
              llvm::CallInst::Create(
                  module.nlex_apply_postag->getFunctionType(),
                  module.nlex_apply_postag, args, "", &insn);
            }
          }
        }
      }
    }
    auto backtrackBB = llvm::BasicBlock::Create(module.TheContext, "_backtrack",
                                                module.main());

    if (!module.backtrackBB)
      module.backtrackBB = backtrackBB;

    module.Builder.SetInsertPoint(backtrackBB);
    module.Builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        module.token_length);
    module.Builder.CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext), 0),
        module.chars_since_last_final);
    // see into the abyss
    module.Builder.CreateCall(
        module.nlex_restore,
        {module.Builder.CreateLoad(module.last_backtrack_branch_position)});

    auto backtrackExitBB = llvm::BasicBlock::Create(
        module.TheContext, "_backtrack_exit", module.main());

    if (!module.backtrackExitBB)
      module.backtrackExitBB = backtrackExitBB;

    module.Builder.SetInsertPoint(backtrackExitBB);
    // see into the abyss
    module.Builder.CreateCall(
        module.nlex_restore,
        {module.Builder.CreateInBoundsGEP(
            module.Builder.CreateCall(module.nlex_current_p),
            {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.TheContext),
                                    -1)})});
    module.Builder.CreateBr(module.BBfinalise);
  }
  void end(const GenLexer &lexer_stuff) {
    using namespace llvm;
    // finish the function
    module.DBuilder->finalize();
    llvm::verifyFunction(*module.main());

    // if we're targeting windows in library mode, add an empty
    // _DllMainCRTStartup because windows
    if (targetTriple.library && targetTriple.triple.getOSName() == "windows") {
      llvm::IRBuilder<> Builder(module.TheContext);
      llvm::Function *x_dll = llvm::Function::Create(
          llvm::FunctionType::get(llvm::Type::getVoidTy(module.TheContext),
                                  false),
          llvm::GlobalValue::LinkageTypes::ExternalLinkage,
          "_DllMainCRTStartup", *module.TheModule);
      // and return
      auto bb = llvm::BasicBlock::Create(module.TheContext, "", x_dll);
      Builder.SetInsertPoint(bb);
      Builder.CreateRetVoid();
    }

    auto Composite = std::make_unique<llvm::Module>(module.TheModule->getName(),
                                                    module.TheContext);
    llvm::Linker L(*Composite);

    if (!targetTriple.library)
      L.linkInModule(
          std::move(module.RTSModule)); // RTS only needed for executable build

    if (lexer_stuff.tagpos.has_value()) {
      L.linkInModule(std::move(
          module.DeserModule)); // Deser only needed if postag is enabled
      slts.show(Display::Type::MUST_SHOW,
                "Please also link against {<magenta>}libc++{<clean>} [This is "
                "a temporary issue]");
    }

    L.linkInModule(std::move(module.TheModule));

    module.TheMPM->run(*Composite);

    legacy::PassManager pass;
    auto FileType = LLVMTargetMachine::CodeGenFileType::CGFT_ObjectFile;
    std::error_code EC;
    if (output_file_name == "")
      output_file_name = std::string{Composite->getName()} + ".o";
    raw_fd_ostream dest(output_file_name, EC, sys::fs::OF_None);

    if (EC) {
      errs() << "Could not open file: " << EC.message();
      return;
    }
    if (!targetTriple._write_ll) {
      if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr,
                                                FileType)) {
        // todo error
      }
      pass.run(*Composite);

      // if we're targeting windows and building a library, export a module
      // definition
      if (targetTriple.library &&
          targetTriple.triple.getOSName() == "windows") {
        auto path = std::filesystem::path{output_file_name}
                        .replace_extension(".def")
                        .string();
        raw_fd_ostream def_dest(path, EC, sys::fs::OF_None);
        if (EC) {
          errs() << "Could not open file: " << EC.message();
          return;
        }
        def_dest << "LIBRARY " << std::filesystem::path{output_file_name}.stem()
                 << "\n";
        def_dest << "EXPORTS\n";

        for (const auto &glob : Composite->globals())
          if (glob.getLinkage() ==
              llvm::GlobalValue::LinkageTypes::ExternalLinkage)
            def_dest << "\t" << glob.getName() << "\n";
        for (const auto &fun : Composite->functions())
          if (fun.getLinkage() ==
                  llvm::GlobalValue::LinkageTypes::ExternalLinkage &&
              !fun.isIntrinsic())
            def_dest << "\t" << fun.getName() << "\n";

        slts.show(Display::Type::MUST_SHOW,
                  "also generated a module-definition file "
                  "{<magenta>}%s{<clean>}, use it to export functions for the "
                  "dll:\n\tlink.exe /dll /def:%s %s",
                  path.c_str(), path.c_str(), output_file_name.c_str());
      }
    } else
      Composite->print(dest, nullptr);
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
