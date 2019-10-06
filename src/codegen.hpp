#include "parser.hpp"

enum class CodegenTarget {
  TargetC,
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
  virtual void generate(DFANode<NFATypeT> *node);
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
  DFACCodeGenerator()
      : CodeGenerator<T>(
            {CodegenStartPhase::DFAPhase, CodegenTarget::TargetC}) {}

  virtual void generate(DFANode<T> *node);
};