$TBB_PREFIX = "F:\Pwogwam\TBB"
$LLVM_PREFIX = "F:\Pwogwam\LLVM"
$XXD = "C:\tools\vim\vim82\xxd.exe"

function test_bc
{
	if ([System.IO.File]::Exists("test_bc.h")) { return }
	clang -g -c rts.c -emit-llvm -o test.bc
	& $XXD -i test.bc test_bc.h
}

function deser
{
	if ([System.IO.File]::Exists("deser.inc")) { return }
	clang++ -g -c deser.inc.cc -emit-llvm -o deser.inc.bc
	& $XXD -i deser.inc.bc deser.inc
}

function kaleid
{
	if ([System.IO.File]::Exists("kaleid.o")) { return }
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs "-I$LLVM_PREFIX/include" "-I$TBB_PREFIX/include" "-L$LLVM_PREFIX/lib" "-L$TBB_PREFIX/lib" kaleid.cpp -D_MD -D_DEBUG -D_DLL -c -g
}
function lexer
{
	if ([System.IO.File]::Exists("lexer.o")) { return }
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs "-I$LLVM_PREFIX/include" "-I$TBB_PREFIX/include" "-L$LLVM_PREFIX/lib" "-L$TBB_PREFIX/lib" lexer.cc -D_MD -D_DEBUG -D_DLL -c -g
}
function parser
{
	if ([System.IO.File]::Exists("parser.o")) { return }
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs "-I$LLVM_PREFIX/include" "-I$TBB_PREFIX/include" "-L$LLVM_PREFIX/lib" "-L$TBB_PREFIX/lib" parser.cc -D_MD -D_DEBUG -D_DLL -c -g
}
function hmm
{
	if ([System.IO.File]::Exists("hmm.o")) { return }
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs "-I$LLVM_PREFIX/include" "-I$TBB_PREFIX/include" "-L$LLVM_PREFIX/lib" "-L$TBB_PREFIX/lib" hmm.cc -D_MD -D_DEBUG -D_DLL -c -g
}

function compile
{
	deser; test_bc; kaleid; lexer; parser; hmm
}

function link
{
	compile
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs "-I$LLVM_PREFIX/include" "-I$TBB_PREFIX/include" "-L$LLVM_PREFIX/lib" "-L$TBB_PREFIX/lib" .\kaleid.o .\lexer.o .\parser.o .\hmm.o -ltbb $($(& "$LLVM_PREFIX/bin/llvm-config.exe" --libs).Split() | % {"-l" + $_.Split(".")[0] }) -D_MD -D_DEBUG -D_DLL -lmsvcrtd -lmsvcprtd
}

link
