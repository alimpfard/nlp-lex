#include "parser.hpp"
#include "vm.hpp"
#include <iostream>
#include <list>

enum class CodegenTarget {
  TargetC,
  TargetNLVM,
};

enum class CodegenStartPhase {
  RegexpPhase, // generates code straight from the regexp
  NFAPhase,
  DFAPhase, // generates code from the optimised DFA generated from the
            // optimised NFA
};

struct CodeGeneratorProperties {
  CodegenStartPhase start_phase;
  CodegenTarget target;
};

template <typename NFATypeT> struct CodeGenerator {
public:
  CodeGeneratorProperties properties;

  CodeGenerator(CodeGeneratorProperties p) : properties(p) {}

  // virtual void generate(NFANode<NFATypeT> *node);
  virtual void
  generate(DFANode<std::set<NFANode<NFATypeT> *>> *node,
           std::set<DFANode<std::set<NFANode<NFATypeT> *>> *> visited = {});
  virtual std::string output(const GenLexer &&lexer_stuff = {});
  // virtual void generate(Regexp *exp);

  void run(NFANode<NFATypeT> *node);
};

// struct RegexpCCodeGenerator : public CodeGenerator {
// public:
//   RegexpCCodeGenerator()
//       : CodeGenerator(
//             {CodegenStartPhase::RegexpPhase, CodegenTarget::TargetC}) {}
//
//   virtual void generate(Regexp *exo);
// };

template <typename T> struct DFACCodeGenerator : public CodeGenerator<T> {
public:
  std::list<std::pair<std::string, std::string>> output_cases;

  DFACCodeGenerator()
      : CodeGenerator<T>(
            {CodegenStartPhase::DFAPhase, CodegenTarget::TargetC}) {}

  virtual void
  generate(DFANode<std::set<NFANode<T> *>> *node,
           std::set<DFANode<std::set<NFANode<T> *>> *> visited = {});
  virtual std::string output(const GenLexer &&lexer_stuff = {});
};
template <typename T> struct DFANLVMCodeGenerator : public CodeGenerator<T> {
public:
  nlvm::Builder builder;
  llvm::BasicBlock *root_bb;
  std::map<int, llvm::Function *> subexprFunc = {};
  std::map<int, DFANode<std::set<NFANode<T> *>> *> subexprs = {};

  DFANLVMCodeGenerator()
      : CodeGenerator<T>(
            {CodegenStartPhase::DFAPhase, CodegenTarget::TargetNLVM}),
        builder("test.nlex") {}

  DFANLVMCodeGenerator(std::string fname, llvm::raw_ostream *out)
      : CodeGenerator<T>(
            {CodegenStartPhase::DFAPhase, CodegenTarget::TargetNLVM}),
        builder(fname, out) {}

  virtual void
  generate(DFANode<std::set<NFANode<T> *>> *node,
           std::set<DFANode<std::set<NFANode<T> *>> *> visited = {});

  virtual void generate(
      DFANode<std::set<NFANode<T> *>> *node,
      std::set<DFANode<std::set<NFANode<T> *>> *> visited,
      std::map<DFANode<std::set<NFANode<T> *>> *, llvm::BasicBlock *> &blocks);
  virtual std::string output(const GenLexer &&lexer_stuff = {});
};
