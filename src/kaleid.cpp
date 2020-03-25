#include "basevm.hpp"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

using namespace llvm;
using namespace llvm::sys;

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,

  // control
  tok_if = -6,
  tok_then = -7,
  tok_else = -8,
  tok_for = -9,
  tok_while = -10,
  tok_in = -11,

  // operators
  tok_binary = -12,
  tok_unary = -13,

  // var definition
  tok_var = -14,
  tok_global = -15,

  // string
  tok_string = -16,

  // extern modifier
  tok_variadic = -17,
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number
static std::string StringValue;   // Filled in if tok_string
static std::string fed_string = "";

void KaleidFeed(std::string s) { fed_string += "\n" + s; }

static char next_char() {
  if (fed_string.size() == 0)
    return 0;
  char c = fed_string[0];
  fed_string = fed_string.substr(1);
  return c;
}

/// gettok - Return the next token from standard input.
static int gettok(bool reset = false) {
  static int LastChar = ' ';
  if (reset)
    LastChar = ' ';
  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = next_char();

  if (LastChar == '"') {
    StringValue = "";
    bool escape = false;
    while ((LastChar = next_char()) != '"') {
      char c = LastChar;
      if (c == '\\') {
        if (escape) {
          escape = false;
          StringValue += c;
        } else
          escape = true;
      } else {
        if (escape) {
          escape = false;
          switch (c) {
          case 'x': {
            char buf[3];
            buf[0] = next_char();
            buf[1] = next_char();
            buf[2] = 0;
            StringValue += (char)strtol(buf, NULL, 16);
            break;
          }
          case 'n':
            StringValue += "\n";
            break;
          case 'r':
            StringValue += "\r";
            break;
          case 'f':
            StringValue += "\f";
            break;
          case 't':
            StringValue += "\t";
            break;
          default:
            StringValue += LastChar;
            break;
          }
        } else
          StringValue += LastChar;
      }
    }

    LastChar = next_char(); // eat the ending '"'
    return tok_string;
  }

  if (isalpha(LastChar) ||
      LastChar == '_') { // identifier: [a-zA-Z_][a-zA-Z_0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = next_char())) || LastChar == '_')
      IdentifierStr += LastChar;

    if (IdentifierStr == "def")
      return tok_def;
    if (IdentifierStr == "extern")
      return tok_extern;
    if (IdentifierStr == "if")
      return tok_if;
    if (IdentifierStr == "then")
      return tok_then;
    if (IdentifierStr == "else")
      return tok_else;
    if (IdentifierStr == "for")
      return tok_for;
    if (IdentifierStr == "while")
      return tok_while;
    if (IdentifierStr == "in")
      return tok_in;
    if (IdentifierStr == "binary")
      return tok_binary;
    if (IdentifierStr == "unary")
      return tok_unary;
    if (IdentifierStr == "var")
      return tok_var;
    if (IdentifierStr == "global")
      return tok_global;
    if (IdentifierStr == "variadic")
      return tok_variadic;
    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.' ||
      LastChar == '-') { // Number: -?[0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = next_char();
    } while (isdigit(LastChar) || LastChar == '.');

    if (NumStr == "-") {
      // This ain't no number
      return NumStr[0]; // just return it as a normal token
    }

    NumVal = strtod(NumStr.c_str(), nullptr);
    return tok_number;
  }

  if (LastChar == '#') {
    // Comment until end of line.
    do
      LastChar = next_char();
    while (LastChar != 0 && LastChar != '\n' && LastChar != '\r');

    if (LastChar != 0)
      return gettok();
  }

  if (LastChar == 0) {
    return tok_eof;
  }

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = next_char();
  return ThisChar;
}

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace {

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() = default;

  virtual Value *codegen() = 0;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
  double Val;

public:
  NumberExprAST(double Val) : Val(Val) {}

  Value *codegen() override;
  double getValue() const { return Val; }
};

class StringExprAST : public ExprAST {
  std::string Val;

public:
  StringExprAST(std::string &&s) : Val(std::move(s)) {}
  Value *codegen() override;
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name) {}

  Value *codegen() override;
  const std::string &getName() const { return Name; }
};

/// UnaryExprAST - Expression class for a unary operator.
class UnaryExprAST : public ExprAST {
  char Opcode;
  std::unique_ptr<ExprAST> Operand;

public:
  UnaryExprAST(char Opcode, std::unique_ptr<ExprAST> Operand)
      : Opcode(Opcode), Operand(std::move(Operand)) {}

  Value *codegen() override;
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

  Value *codegen() override;
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : Callee(Callee), Args(std::move(Args)) {}

  Value *codegen() override;
};

/// IfExprAST - Expression class for if/then/else.
class IfExprAST : public ExprAST {
  std::unique_ptr<ExprAST> Cond, Then, Else;

public:
  IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then,
            std::unique_ptr<ExprAST> Else)
      : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}

  Value *codegen() override;
};

/// ForExprAST - Expression class for for/in.
class ForExprAST : public ExprAST {
  std::string VarName;
  std::unique_ptr<ExprAST> Start, End, Step, Body;

public:
  ForExprAST(const std::string &VarName, std::unique_ptr<ExprAST> Start,
             std::unique_ptr<ExprAST> End, std::unique_ptr<ExprAST> Step,
             std::unique_ptr<ExprAST> Body)
      : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
        Step(std::move(Step)), Body(std::move(Body)) {}

  Value *codegen() override;
};

/// WhileExprAST - Expression class for while/in.
class WhileExprAST : public ExprAST {
  std::unique_ptr<ExprAST> Condition, Body;

public:
  WhileExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Body)
      : Condition(std::move(Cond)), Body(std::move(Body)) {}

  Value *codegen() override;
};

/// VarExprAST - Expression class for var/in
class VarExprAST : public ExprAST {
  std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames;
  std::unique_ptr<ExprAST> Body;

public:
  VarExprAST(
      std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames,
      std::unique_ptr<ExprAST> Body)
      : VarNames(std::move(VarNames)), Body(std::move(Body)) {}

  Value *codegen() override;
};

/// GlobalExprAST - Expression class for global
class GlobalExprAST : public ExprAST {
  std::vector<std::pair<std::string, Constant *>> Names;

public:
  GlobalExprAST(std::vector<std::pair<std::string, Constant *>> Names)
      : Names(std::move(Names)) {}

  Value *codegen() override;
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes), as well as if it is an operator.
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;
  bool IsOperator;
  unsigned Precedence; // Precedence if a binary op.
  bool IsVariadic;

public:
  PrototypeAST(const std::string &Name, std::vector<std::string> Args,
               bool IsOperator = false, unsigned Prec = 0, bool IsVar = false)
      : Name(Name), Args(std::move(Args)), IsOperator(IsOperator),
        Precedence(Prec), IsVariadic(IsVar) {}

  Function *codegen();
  const std::string &getName() const { return Name; }

  bool isUnaryOp() const { return IsOperator && Args.size() == 1; }
  bool isBinaryOp() const { return IsOperator && Args.size() == 2; }
  bool isVariadic() const { return !IsOperator && IsVariadic; }

  char getOperatorName() const {
    assert(isUnaryOp() || isBinaryOp());
    return Name[Name.size() - 1];
  }

  unsigned getBinaryPrecedence() const { return Precedence; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}

  Function *codegen();
};

} // end anonymous namespace

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
static int getNextToken(bool reset = false) { return CurTok = gettok(reset); }

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;

/// Module
/// Stolen from Codegen, required here
static nlvm::BaseModule *mModule;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
  if (!isascii(CurTok))
    return -1;

  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}
/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const std::string &Str) {
  fprintf(stderr, "Error: %s\n", Str.c_str());
  return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression();

/// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(NumVal);
  getNextToken(); // consume the number
  return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
  getNextToken(); // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (CurTok != ')')
    return LogError("expected ')'");
  getNextToken(); // eat ).
  return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
  std::string IdName = IdentifierStr;

  getNextToken(); // eat identifier.

  if (CurTok != '(') // Simple variable ref.
    return std::make_unique<VariableExprAST>(IdName);

  // Call.
  getNextToken(); // eat (
  std::vector<std::unique_ptr<ExprAST>> Args;
  if (CurTok != ')') {
    while (true) {
      if (auto Arg = ParseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (CurTok == ')')
        break;

      if (CurTok != ',')
        return LogError("Expected ')' or ',' in argument list");
      getNextToken();
    }
  }

  // Eat the ')'.
  getNextToken();

  return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// ifexpr ::= 'if' expression 'then' expression 'else' expression
static std::unique_ptr<ExprAST> ParseIfExpr() {
  getNextToken(); // eat the if.

  // condition.
  auto Cond = ParseExpression();
  if (!Cond)
    return nullptr;

  if (CurTok != tok_then)
    return LogError("expected then");
  getNextToken(); // eat the then

  auto Then = ParseExpression();
  if (!Then)
    return nullptr;

  if (CurTok != tok_else)
    return LogError("expected else");

  getNextToken();

  auto Else = ParseExpression();
  if (!Else)
    return nullptr;

  return std::make_unique<IfExprAST>(std::move(Cond), std::move(Then),
                                     std::move(Else));
}

/// whileexpr ::= 'while' expr in expr
static std::unique_ptr<ExprAST> ParseWhileExpr() {
  getNextToken(); // eat the while

  auto Cond = ParseExpression();
  if (!Cond)
    return nullptr;

  if (CurTok != tok_in)
    return LogError("expected 'in' after while");
  getNextToken(); // eat 'in'.

  auto Body = ParseExpression();
  if (!Body)
    return nullptr;

  return std::make_unique<WhileExprAST>(std::move(Cond), std::move(Body));
}

/// forexpr ::= 'for' identifier '=' expr ',' expr (',' expr)? 'in' expression
static std::unique_ptr<ExprAST> ParseForExpr() {
  getNextToken(); // eat the for.

  if (CurTok != tok_identifier)
    return LogError("expected identifier after for");

  std::string IdName = IdentifierStr;
  getNextToken(); // eat identifier.

  if (CurTok != '=')
    return LogError("expected '=' after for");
  getNextToken(); // eat '='.

  auto Start = ParseExpression();
  if (!Start)
    return nullptr;
  if (CurTok != ',')
    return LogError("expected ',' after for start value");
  getNextToken();

  auto End = ParseExpression();
  if (!End)
    return nullptr;

  // The step value is optional.
  std::unique_ptr<ExprAST> Step;
  if (CurTok == ',') {
    getNextToken();
    Step = ParseExpression();
    if (!Step)
      return nullptr;
  }

  if (CurTok != tok_in)
    return LogError("expected 'in' after for");
  getNextToken(); // eat 'in'.

  auto Body = ParseExpression();
  if (!Body)
    return nullptr;

  return std::make_unique<ForExprAST>(IdName, std::move(Start), std::move(End),
                                      std::move(Step), std::move(Body));
}

/// globalexpr ::= 'global' identifier ('=' number)? (, identifier ('='
/// number)?)*
static std::unique_ptr<ExprAST> ParseGlobalExpr() {
  getNextToken(); // eat the global

  std::vector<std::pair<std::string, Constant *>> Names;

  // at least one global name is required
  if (CurTok != tok_identifier)
    return LogError("expected identifier after global");

  while (true) {
    std::string Name = IdentifierStr;
    getNextToken(); // eat identifier

    Constant *init = nullptr;
    if (CurTok == '=') {
      getNextToken(); // eat the '='

      std::unique_ptr<ExprAST> number = nullptr;
      number = ParseNumberExpr();

      if (!number)
        return nullptr;

      NumberExprAST *nast = (NumberExprAST *)number.get();
      double value = nast->getValue();
      init = ConstantFP::get(mModule->TheContext, APFloat{value});
    }
    Names.push_back({Name, init});

    if (CurTok != ',')
      break;
    getNextToken();

    if (CurTok != tok_identifier)
      return LogError("expected identifier list after global");
  }

  return std::make_unique<GlobalExprAST>(std::move(Names));
}

/// varexpr ::= 'var' identifier ('=' expression)?
//                    (',' identifier ('=' expression)?)* 'in' expression
static std::unique_ptr<ExprAST> ParseVarExpr() {
  getNextToken(); // eat the var.

  std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames;

  // At least one variable name is required.
  if (CurTok != tok_identifier)
    return LogError("expected identifier after var");

  while (true) {
    std::string Name = IdentifierStr;
    getNextToken(); // eat identifier.

    // Read the optional initializer.
    std::unique_ptr<ExprAST> Init = nullptr;
    if (CurTok == '=') {
      getNextToken(); // eat the '='.

      Init = ParseExpression();
      if (!Init)
        return nullptr;
    }

    VarNames.push_back(std::make_pair(Name, std::move(Init)));

    // End of var list, exit loop.
    if (CurTok != ',')
      break;
    getNextToken(); // eat the ','.

    if (CurTok != tok_identifier)
      return LogError("expected identifier list after var");
  }

  // At this point, we have to have 'in'.
  if (CurTok != tok_in)
    return LogError("expected 'in' keyword after 'var'");
  getNextToken(); // eat 'in'.

  auto Body = ParseExpression();
  if (!Body)
    return nullptr;

  return std::make_unique<VarExprAST>(std::move(VarNames), std::move(Body));
}

static std::unique_ptr<ExprAST> ParseString() {
  std::string stringvalue = StringValue;
  getNextToken(); // eat string
  return std::make_unique<StringExprAST>(std::move(stringvalue));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
///   ::= ifexpr
///   ::= forexpr
///   ::= varexpr
///   ::= string
static std::unique_ptr<ExprAST> ParsePrimary() {
  switch (CurTok) {
  default:
    return LogError("unknown token " + std::to_string((char)CurTok) +
                    " when expecting an expression");
  case tok_identifier:
    return ParseIdentifierExpr();
  case tok_number:
    return ParseNumberExpr();
  case '(':
    return ParseParenExpr();
  case tok_if:
    return ParseIfExpr();
  case tok_for:
    return ParseForExpr();
  case tok_while:
    return ParseWhileExpr();
  case tok_var:
    return ParseVarExpr();
  case tok_global:
    return ParseGlobalExpr();
  case tok_string:
    return ParseString();
  }
}

/// unary
///   ::= primary
///   ::= '!' unary
static std::unique_ptr<ExprAST> ParseUnary() {
  // If the current token is not an operator, it must be a primary expr.
  if (!isascii(CurTok) || CurTok == '(' || CurTok == ',')
    return ParsePrimary();

  // If this is a unary operator, read it.
  int Opc = CurTok;
  getNextToken();
  if (auto Operand = ParseUnary())
    return std::make_unique<UnaryExprAST>(Opc, std::move(Operand));
  return nullptr;
}

/// binoprhs
///   ::= ('+' unary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) {
  // If this is a binop, find its precedence.
  while (true) {
    int TokPrec = GetTokPrecedence();

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;

    // Okay, we know this is a binop.
    int BinOp = CurTok;
    getNextToken(); // eat binop

    // Parse the unary expression after the binary operator.
    auto RHS = ParseUnary();
    if (!RHS)
      return nullptr;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS =
        std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

/// expression
///   ::= unary binoprhs
///
static std::unique_ptr<ExprAST> ParseExpression() {
  auto LHS = ParseUnary();
  if (!LHS)
    return nullptr;

  return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
///   ::= id '(' id* ')'
///   ::= binary LETTER number? (id, id)
///   ::= unary LETTER (id)
static std::unique_ptr<PrototypeAST> ParsePrototype(bool variadic = false) {
  std::string FnName;

  unsigned Kind = 0; // 0 = identifier, 1 = unary, 2 = binary.
  unsigned BinaryPrecedence = 30;

  switch (CurTok) {
  default:
    return LogErrorP("Expected function name in prototype");
  case tok_identifier:
    FnName = IdentifierStr;
    Kind = 0;
    getNextToken();
    break;
  case tok_unary:
    getNextToken();
    if (!isascii(CurTok))
      return LogErrorP("Expected unary operator");
    FnName = "unary";
    FnName += (char)CurTok;
    Kind = 1;
    getNextToken();
    break;
  case tok_binary:
    getNextToken();
    if (!isascii(CurTok))
      return LogErrorP("Expected binary operator");
    FnName = "binary";
    FnName += (char)CurTok;
    Kind = 2;
    getNextToken();

    // Read the precedence if present.
    if (CurTok == tok_number) {
      if (NumVal < 1 || NumVal > 100)
        return LogErrorP("Invalid precedence: must be 1..100");
      BinaryPrecedence = (unsigned)NumVal;
      getNextToken();
    }
    break;
  }

  if (CurTok != '(')
    return LogErrorP("Expected '(' in prototype");

  std::vector<std::string> ArgNames;
  while (getNextToken() == tok_identifier)
    ArgNames.push_back(IdentifierStr);
  if (CurTok != ')')
    return LogErrorP("Expected ')' in prototype");

  // success.
  getNextToken(); // eat ')'.

  // Verify right number of names for operator.
  if (Kind && ArgNames.size() != Kind)
    return LogErrorP("Invalid number of operands for operator");

  return std::make_unique<PrototypeAST>(FnName, ArgNames, Kind != 0,
                                        BinaryPrecedence, variadic);
}

/// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition() {
  getNextToken(); // eat def.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  if (auto E = ParseExpression())
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  return nullptr;
}

/// toplevelexpr ::= expression
static std::unique_ptr<ExprAST> ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    return E;
  }
  return nullptr;
}

/// external ::= 'extern' 'variadic'? prototype
static std::unique_ptr<PrototypeAST> ParseExtern() {
  getNextToken(); // eat extern.
  auto var = false;
  if (CurTok == tok_variadic) {
    var = true;
    getNextToken(); // eat 'variadic
  }
  return ParsePrototype(var);
}

//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

static IRBuilder<> *Builder;
static std::map<std::string, AllocaInst *> NamedValues;
static std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

static Value *castfromdbl(Value *inst, Type *target) {
  auto instT = inst->getType();
  if (instT != target) {
    auto i64T = llvm::Type::getInt64Ty(mModule->TheContext);
    if (target->isArrayTy()) {
      inst = (*Builder).CreateFPToSI(inst, i64T);
      inst = (*Builder).CreateIntToPtr(
          inst, target->getArrayElementType()->getPointerTo());
      inst = (*Builder).CreateBitCast(inst, target);
    } else if (target->isPointerTy()) {
      inst = (*Builder).CreateFPToSI(inst, i64T);
      inst = (*Builder).CreateIntToPtr(inst, target);
    } else
      inst = llvm::CastInst::Create(Instruction::FPToSI, inst, target, "rcast",
                                    (*Builder).GetInsertBlock());
  }
  return inst;
}
static Value *cast2dbl(Value *inst) {
  auto instT = inst->getType();
  auto dblT = llvm::Type::getDoubleTy(mModule->TheContext);
  if (instT != dblT) {
    auto i64T = llvm::Type::getInt64Ty(mModule->TheContext);
    if (instT->isArrayTy()) {
      inst = (*Builder).CreateBitCast(inst, i64T);
      instT = inst->getType();
    }
    if (instT->isPointerTy())
      inst = (*Builder).CreatePtrToInt(inst, i64T);
    inst = llvm::CastInst::Create(Instruction::SIToFP, inst, dblT, "mcast",
                                  (*Builder).GetInsertBlock());
  }
  return inst;
}

Value *LogErrorV(const char *Str) {
  LogError(Str);
  return nullptr;
}

Function *getFunction(std::string Name) {
  // First, see if the function has already been added to the current module.
  if (auto *F = (mModule->TheModule)->getFunction(Name))
    return F;

  // If not, check whether we can codegen the declaration from some existing
  // prototype.
  auto FI = FunctionProtos.find(Name);
  if (FI != FunctionProtos.end())
    return FI->second->codegen();

  // If no existing prototype exists, return null.
  return nullptr;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                          const std::string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getDoubleTy((mModule->TheContext)), nullptr,
                           VarName);
}

Value *NumberExprAST::codegen() {
  return ConstantFP::get((mModule->TheContext), APFloat(Val));
}

Value *StringExprAST::codegen() {
  std::vector<Constant *> ref;
  for (int i = 0; i < Val.size(); i++)
    ref.push_back(
        ConstantInt::get(Type::getInt8Ty(mModule->TheContext), Val[i]));
  ref.push_back(ConstantInt::get(Type::getInt8Ty(mModule->TheContext), 0));
  GlobalVariable *stringVal = new GlobalVariable{
      *mModule->TheModule,
      ArrayType::get(Type::getInt8Ty(mModule->TheContext), Val.size() + 1),
      true,
      GlobalValue::LinkageTypes::InternalLinkage,
      ConstantArray::get(
          ArrayType::get(Type::getInt8Ty(mModule->TheContext), Val.size() + 1),
          ref),
      "@userstring"};
  llvm::Value *idxs[] = {
      reinterpret_cast<llvm::Value *>(llvm::ConstantInt::get(
          llvm::Type::getInt64Ty(mModule->TheContext), 0, false)),
      reinterpret_cast<llvm::Value *>(llvm::ConstantInt::get(
          llvm::Type::getInt64Ty(mModule->TheContext), 0, false)),
  };
  return cast2dbl(ConstantExpr::getInBoundsGetElementPtr(
      ArrayType::get(Type::getInt8Ty(mModule->TheContext), Val.size() + 1),
      stringVal, idxs));
}

Value *VariableExprAST::codegen() {
  // Look this variable up in the function.
  Value *V = NamedValues[Name];

  if (!V) {
    if ((V = mModule->TheModule->getGlobalVariable(Name, true)) == nullptr)
      return LogErrorV(("Unknown variable name " + Name).c_str());
  }

  // Load the value.
  if (V->getType()->isPointerTy() &&
      V->getType()->getPointerElementType()->isArrayTy()) {
    llvm::Value *idxs[] = {
        reinterpret_cast<llvm::Value *>(llvm::ConstantInt::get(
            llvm::Type::getInt64Ty(mModule->TheContext), 0, false)),
        reinterpret_cast<llvm::Value *>(llvm::ConstantInt::get(
            llvm::Type::getInt64Ty(mModule->TheContext), 0, false)),
    };
    return cast2dbl(
        (*Builder).CreateInBoundsGEP(V->getType()->getPointerElementType(), V,
                                     idxs)); // load arrays by pointer
  }
  return cast2dbl((*Builder).CreateLoad(V, Name.c_str()));
}

Value *UnaryExprAST::codegen() {
  Value *OperandV = Operand->codegen();
  if (!OperandV)
    return nullptr;

  Function *F = getFunction(std::string("unary") + Opcode);
  if (!F)
    return LogErrorV(
        std::string{"Unknown unary operator " + std::string{Opcode}}.c_str());

  return (*Builder).CreateCall(F, OperandV, "unop");
}

Value *BinaryExprAST::codegen() {
  // Special case '=' because we don't want to emit the LHS as an expression.
  if (Op == '=') {
    // Assignment requires the LHS to be an identifier.
    // This assume we're building without RTTI because LLVM builds that way by
    // default.  If you build LLVM with RTTI this can be changed to a
    // dynamic_cast for automatic error checking.
    VariableExprAST *LHSE = static_cast<VariableExprAST *>(LHS.get());
    if (!LHSE)
      return LogErrorV("destination of '=' must be a variable");
    // Codegen the RHS.
    Value *Val = RHS->codegen();
    if (!Val)
      return nullptr;

    // Look up the name.
    Value *Variable = NamedValues[LHSE->getName()];
    if (!Variable)
      if ((Variable = mModule->TheModule->getGlobalVariable(LHSE->getName(),
                                                            true)) == nullptr)
        return LogErrorV(("Unknown variable name " + LHSE->getName()).c_str());

    auto val = Val;

    if (Variable->getType()->getPointerElementType() != val->getType()) {
      // cast back
      val = castfromdbl(val, Variable->getType()->getPointerElementType());
    }
    (*Builder).CreateStore(val, Variable);
    return Val;
  }

  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  switch (Op) {
  case '+':
    return (*Builder).CreateFAdd(L, R, "addtmp");
  case '-':
    return (*Builder).CreateFSub(L, R, "subtmp");
  case '*':
    return (*Builder).CreateFMul(L, R, "multmp");
  case '/':
    return (*Builder).CreateFDiv(L, R, "divtmp");
  case '<':
    L = (*Builder).CreateFCmpULT(L, R, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return (*Builder).CreateUIToFP(L, Type::getDoubleTy((mModule->TheContext)),
                                   "booltmp");
  case '>':
    L = (*Builder).CreateFCmpUGT(L, R, "cmptmp");
    return (*Builder).CreateUIToFP(L, Type::getDoubleTy((mModule->TheContext)),
                                   "booltmp");
  case '?':
    L = (*Builder).CreateFCmpUEQ(L, R, "cmptmp");
    return (*Builder).CreateUIToFP(L, Type::getDoubleTy((mModule->TheContext)),
                                   "booltmp");
  case '!':
    L = (*Builder).CreateFCmpUNE(L, R, "cmptmp");
    return (*Builder).CreateUIToFP(L, Type::getDoubleTy((mModule->TheContext)),
                                   "booltmp");
  default:
    break;
  }

  // If it wasn't a builtin binary operator, it must be a user defined one. Emit
  // a call to it.
  Function *F = getFunction(std::string("binary") + Op);
  assert(F && "binary operator not found!");

  Value *Ops[] = {L, R};
  return (*Builder).CreateCall(F, Ops, "binop");
}

Value *CallExprAST::codegen() {
  // Look up the name in the global module table.
  Function *CalleeF = getFunction(Callee);
  if (!CalleeF)
    return LogErrorV(
        (std::string{"Unknown function `"} + Callee + "' referenced").c_str());

  // If argument mismatch error.
  if (!CalleeF->isVarArg() && CalleeF->arg_size() < Args.size())
    return LogErrorV((std::string{"Incorrect # arguments passed to " + Callee +
                                  " (Expected "} +
                      std::to_string(CalleeF->arg_size()) + " but got " +
                      std::to_string(Args.size()) + ")")
                         .c_str());

  if (CalleeF->arg_size() > Args.size())
    return LogErrorV((std::string{"Incorrect # arguments passed to " + Callee +
                                  " (Expected "} +
                      std::to_string(CalleeF->arg_size()) + " but got " +
                      std::to_string(Args.size()) + ")")
                         .c_str());

  std::vector<Value *> ArgsV;
  // auto farg = CalleeF->arg_begin();
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    // if (farg == CalleeF->arg_end())
    ArgsV.push_back(Args[i]->codegen());
    // else {
    // ArgsV.push_back(castfromdbl(Args[i]->codegen(), farg->getType()));
    // farg++;
    // }
    if (!ArgsV.back())
      return nullptr;
  }

  return cast2dbl((*Builder).CreateCall(CalleeF, ArgsV, "calltmp"));
}

Value *IfExprAST::codegen() {
  Value *CondV = Cond->codegen();
  if (!CondV)
    return nullptr;

  // Convert condition to a bool by comparing non-equal to 0.0.
  CondV = (*Builder).CreateFCmpONE(
      CondV, ConstantFP::get((mModule->TheContext), APFloat(0.0)), "ifcond");

  Function *TheFunction = (*Builder).GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  BasicBlock *ThenBB =
      BasicBlock::Create((mModule->TheContext), "then", TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create((mModule->TheContext), "else");
  BasicBlock *MergeBB = BasicBlock::Create((mModule->TheContext), "ifcont");

  (*Builder).CreateCondBr(CondV, ThenBB, ElseBB);

  // Emit then value.
  (*Builder).SetInsertPoint(ThenBB);

  Value *ThenV = Then->codegen();
  if (!ThenV)
    return nullptr;

  (*Builder).CreateBr(MergeBB);
  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = (*Builder).GetInsertBlock();

  // Emit else block.
  TheFunction->getBasicBlockList().push_back(ElseBB);
  (*Builder).SetInsertPoint(ElseBB);

  Value *ElseV = Else->codegen();
  if (!ElseV)
    return nullptr;

  (*Builder).CreateBr(MergeBB);
  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
  ElseBB = (*Builder).GetInsertBlock();

  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  (*Builder).SetInsertPoint(MergeBB);
  PHINode *PN = (*Builder).CreatePHI(Type::getDoubleTy((mModule->TheContext)),
                                     2, "iftmp");

  PN->addIncoming(ThenV, ThenBB);
  PN->addIncoming(ElseV, ElseBB);
  return PN;
}

// Output while-loop as:
// loopcond:
//   ... condition
//   br condition, loop, outloop
// loop:
//   ...
//   bodyexpr
//   ...
//   br loopcond
// outloop:
Value *WhileExprAST::codegen() {
  Function *TheFunction = (*Builder).GetInsertBlock()->getParent();

  // Make the new basic block for the loop header, inserting after current
  // block.
  BasicBlock *LoopBB =
      BasicBlock::Create((mModule->TheContext), "wloopcond", TheFunction);
  BasicBlock *LoopBBB =
      BasicBlock::Create((mModule->TheContext), "wloop", TheFunction);

  // Insert an explicit fall through from the current block to the LoopBB.
  (*Builder).CreateBr(LoopBB);

  // Start insertion in LoopBB.
  (*Builder).SetInsertPoint(LoopBB);

  // Emit the condition of the loop
  auto CondVal = Condition->codegen();
  if (!CondVal)
    return nullptr;

  // Create the "after loop" block and insert it.
  BasicBlock *AfterBB =
      BasicBlock::Create((mModule->TheContext), "wafterloop", TheFunction);

  // Jump to the body if condition ok
  (*Builder).CreateCondBr(
      (*Builder).CreateFCmpONE(
          CondVal, ConstantFP::get((mModule->TheContext), APFloat(0.0)),
          "wloopcond"),
      LoopBBB, AfterBB);

  // Start insertion in LoopBBB.
  (*Builder).SetInsertPoint(LoopBBB);

  // Emit the body of the loop.  This, like any other expr, can change the
  // current BB.  Note that we ignore the value computed by the body, but don't
  // allow an error.
  if (!Body->codegen())
    return nullptr;

  // jump back to condition
  (*Builder).CreateBr(LoopBB);

  // Switch to insertion inside After
  (*Builder).SetInsertPoint(AfterBB);

  // while expr always returns 0.0.
  return Constant::getNullValue(Type::getDoubleTy((mModule->TheContext)));
}
// Output for-loop as:
//   var = alloca double
//   ...
//   start = startexpr
//   store start -> var
//   goto check
// check:
//   endcond = endexpr
//   br endcond, outloop, loop
// loop:
//   ...
//   bodyexpr
//   ...
//   br loopnext
// loopnext:
//   step = stepexpr
//
//   curvar = load var
//   nextvar = curvar + step
//   store nextvar -> var
//   br check
// outloop:
Value *ForExprAST::codegen() {
  Function *TheFunction = (*Builder).GetInsertBlock()->getParent();

  // Create an alloca for the variable in the entry block.
  AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, VarName);

  // Emit the start code first, without 'variable' in scope.
  Value *StartVal = Start->codegen();
  if (!StartVal)
    return nullptr;

  // Store the value into the alloca.
  (*Builder).CreateStore(StartVal, Alloca);

  // Make the new basic block for the loop header, inserting after current
  // block.
  BasicBlock *LoopBB =
      BasicBlock::Create((mModule->TheContext), "loop", TheFunction);
  BasicBlock *CondBB =
      BasicBlock::Create((mModule->TheContext), "loopcond", TheFunction);
  BasicBlock *LoopNextBB =
      BasicBlock::Create((mModule->TheContext), "loopnext", TheFunction);
  BasicBlock *AfterBB =
      BasicBlock::Create((mModule->TheContext), "afterloop", TheFunction);

  // Insert an explicit fall through from the current block to the LoopBB.
  (*Builder).CreateBr(CondBB);

  // Start insertion in LoopBB.
  (*Builder).SetInsertPoint(LoopBB);

  // Within the loop, the variable is defined equal to the PHI node.  If it
  // shadows an existing variable, we have to restore it, so save it now.
  AllocaInst *OldVal = NamedValues[VarName];
  NamedValues[VarName] = Alloca;

  // Emit the body of the loop.  This, like any other expr, can change the
  // current BB.  Note that we ignore the value computed by the body, but don't
  // allow an error.
  if (!Body->codegen())
    return nullptr;

  (*Builder).CreateBr(LoopNextBB);

  (*Builder).SetInsertPoint(CondBB);

  // Compute the end condition.
  Value *EndCond = End->codegen();
  if (!EndCond)
    return nullptr;

  // Convert condition to a bool by comparing non-equal to 0.0.
  EndCond = (*Builder).CreateFCmpONE(
      EndCond, ConstantFP::get((mModule->TheContext), APFloat(0.0)),
      "loopcondv");

  // Insert the conditional branch into the end.
  (*Builder).CreateCondBr(EndCond, LoopBB, AfterBB);

  (*Builder).SetInsertPoint(LoopNextBB);

  // Emit the step value.
  Value *StepVal = nullptr;
  if (Step) {
    StepVal = Step->codegen();
    if (!StepVal)
      return nullptr;
  } else {
    // If not specified, use 1.0.
    StepVal = ConstantFP::get((mModule->TheContext), APFloat(1.0));
  }

  // Reload, increment, and restore the alloca.  This handles the case where
  // the body of the loop mutates the variable.
  Value *CurVar = (*Builder).CreateLoad(Alloca, VarName.c_str());
  Value *NextVar = (*Builder).CreateFAdd(CurVar, StepVal, "nextvar");
  (*Builder).CreateStore(NextVar, Alloca);

  // jump and test the condition
  (*Builder).CreateBr(CondBB);

  // Any new code will be inserted in AfterBB.
  (*Builder).SetInsertPoint(AfterBB);

  // Restore the unshadowed variable.
  if (OldVal)
    NamedValues[VarName] = OldVal;
  else
    NamedValues.erase(VarName);

  // for expr always returns 0.0.
  return Constant::getNullValue(Type::getDoubleTy((mModule->TheContext)));
}

Value *GlobalExprAST::codegen() {
  // Register all globals that don't exist
  for (auto [name, initialiser] : Names) {
    if (mModule->TheModule->getGlobalVariable(name) != nullptr)
      continue;

    new GlobalVariable{*mModule->TheModule,
                       llvm::Type::getDoubleTy(mModule->TheContext),
                       false,
                       GlobalValue::LinkageTypes::InternalLinkage,
                       initialiser,
                       name};
  }
  return Constant::getNullValue(Type::getDoubleTy((mModule->TheContext)));
}

Value *VarExprAST::codegen() {
  std::vector<AllocaInst *> OldBindings;

  Function *TheFunction = (*Builder).GetInsertBlock()->getParent();

  // Register all variables and emit their initializer.
  for (unsigned i = 0, e = VarNames.size(); i != e; ++i) {
    const std::string &VarName = VarNames[i].first;
    ExprAST *Init = VarNames[i].second.get();

    // Emit the initializer before adding the variable to scope, this prevents
    // the initializer from referencing the variable itself, and permits stuff
    // like this:
    //  var a = 1 in
    //    var a = a in ...   # refers to outer 'a'.
    Value *InitVal;
    if (Init) {
      InitVal = Init->codegen();
      if (!InitVal)
        return nullptr;
    } else { // If not specified, use 0.0.
      InitVal = ConstantFP::get((mModule->TheContext), APFloat(0.0));
    }

    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, VarName);
    (*Builder).CreateStore(InitVal, Alloca);

    // Remember the old variable binding so that we can restore the binding when
    // we unrecurse.
    OldBindings.push_back(NamedValues[VarName]);

    // Remember this binding.
    NamedValues[VarName] = Alloca;
  }

  // Codegen the body, now that all vars are in scope.
  Value *BodyVal = Body->codegen();
  if (!BodyVal)
    return nullptr;

  // Pop all our variables from scope.
  for (unsigned i = 0, e = VarNames.size(); i != e; ++i)
    NamedValues[VarNames[i].first] = OldBindings[i];

  // Return the body computation.
  return BodyVal;
}

Function *PrototypeAST::codegen() {
  // Make the function type:  double(double,double) etc.
  std::vector<Type *> Doubles(Args.size(),
                              Type::getDoubleTy((mModule->TheContext)));
  FunctionType *FT = FunctionType::get(Type::getDoubleTy((mModule->TheContext)),
                                       Doubles, isVariadic());

  Function *F = Function::Create(FT, Function::ExternalLinkage, Name,
                                 (mModule->TheModule).get());

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++]);

  return F;
}

Function *FunctionAST::codegen() {
  // Transfer ownership of the prototype to the FunctionProtos map, but keep a
  // reference to it for use below.
  auto &P = *Proto;
  FunctionProtos[Proto->getName()] = std::move(Proto);
  Function *TheFunction = getFunction(P.getName());
  if (!TheFunction)
    return nullptr;

  // If this is an operator, install it.
  if (P.isBinaryOp())
    BinopPrecedence[P.getOperatorName()] = P.getBinaryPrecedence();

  // Create a new basic block to start insertion into.
  BasicBlock *BB =
      BasicBlock::Create((mModule->TheContext), "entry", TheFunction);
  (*Builder).SetInsertPoint(BB);

  // Record the function arguments in the NamedValues map.
  NamedValues.clear();
  for (auto &Arg : TheFunction->args()) {
    // Create an alloca for this variable.
    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName());

    // Store the initial value into the alloca.
    (*Builder).CreateStore(&Arg, Alloca);

    // Add arguments to variable symbol table.
    NamedValues[Arg.getName()] = Alloca;
  }

  if (Value *RetVal = Body->codegen()) {
    // Finish off the function.
    (*Builder).CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);

    return TheFunction;
  }

  // Error reading body, remove function.
  TheFunction->eraseFromParent();

  if (P.isBinaryOp())
    BinopPrecedence.erase(P.getOperatorName());
  return nullptr;
}

//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//

static void HandleDefinition() {
  if (auto FnAST = ParseDefinition()) {
    if (auto *FnIR = FnAST->codegen()) {
      // fprintf(stderr, "Read function definition:");
      // FnIR->print(errs());
      // fprintf(stderr, "\n");
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleExtern() {
  if (auto ProtoAST = ParseExtern()) {
    if (auto *FnIR = ProtoAST->codegen()) {
      // fprintf(stderr, "Read extern: ");
      // FnIR->print(errs());
      // fprintf(stderr, "\n");
      FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression(bool allow_bare) {
  // Evaluate a top-level expression into an anonymous function.
  if (auto FnAST = ParseTopLevelExpr()) {
    if (allow_bare)
      FnAST->codegen();
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
void KaleidCompile(std::string code, llvm::IRBuilder<> &TheBuilder,
                   bool allow_bare_expressions = false) {
  Builder = &TheBuilder;
  KaleidFeed(code);
  getNextToken(true);
  while (true) {
    switch (CurTok) {
    case tok_eof:
      return;
    case ';': // ignore top-level semicolons.
      getNextToken();
      break;
    case tok_def: {
      const auto ip = TheBuilder.saveIP();
      HandleDefinition();
      TheBuilder.restoreIP(ip);
      break;
    }
    case tok_extern: {
      const auto ip = TheBuilder.saveIP();
      HandleExtern();
      TheBuilder.restoreIP(ip);
      break;
    }
    default: {
      HandleTopLevelExpression(allow_bare_expressions);
      break;
    }
    }
  }
}

//===----------------------------------------------------------------------===//
// "Library" functions that can be "extern'd" from user code.
//===----------------------------------------------------------------------===//

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/// putchard - putchar that takes a double and returns 0.
extern "C" DLLEXPORT double putchard(double X) {
  fputc((char)X, stderr);
  return 0;
}

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT double printd(double X) {
  fprintf(stderr, "%f\n", X);
  return 0;
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
static bool initd = false;

void KaleidInitialise(std::string startup_code, nlvm::BaseModule *module) {
  if (initd)
    return;
  initd = true;
  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence['='] = 5;
  BinopPrecedence['<'] = 10;
  BinopPrecedence['>'] = 10;
  BinopPrecedence['?'] = 10;
  BinopPrecedence['!'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40; // highest.
  BinopPrecedence['/'] = 40; // highest.

  mModule = module;
  llvm::IRBuilder<> TheBuilder(mModule->TheContext);
  Builder = &TheBuilder;

  // Prime the first token
  KaleidFeed(startup_code);

  // Initialize the target registry etc.
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  auto TargetTriple = sys::getDefaultTargetTriple();
  (mModule->TheModule)->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    errs() << Error;
    return;
  }

  auto CPU = "generic";
  auto Features = "";

  TargetOptions opt;
  auto RM = Optional<Reloc::Model>();
  auto TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

  (mModule->TheModule)->setDataLayout(TheTargetMachine->createDataLayout());
}

#ifdef TEST
auto main() -> int {
  nlvm::BaseModule m;
  m.TheModule = std::move(std::make_unique<llvm::Module>("shit", m.TheContext));

  KaleidInitialise("", &m);
  llvm::IRBuilder<> builder(m.TheContext);
  std::string line;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, line);

    KaleidCompile(line, builder, true);
    if (line == "")
      break;
  }
  m.TheModule->print(llvm::errs(), nullptr);
  return 0;
}
#endif
