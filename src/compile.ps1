$TBB_PREFIX = "F:\Pwogwam\TBB"
$LLVM_PREFIX = "F:\Pwogwam\LLVM"

function kaleid
{
	if ([System.IO.File]::Exists("kaleid.o")) { return }
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs "-I$LLVM_PREFIX/include" "-I$TBB_PREFIX/include" "-L$LLVM_PREFIX/lib" "-L$TBB_PREFIX/lib" kaleid.cc -D_MD -D_DEBUG -D_DLL -c -g
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

function compile
{
	kaleid; lexer; parser;
}

function link
{
	compile
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs "-I$LLVM_PREFIX/include" "-I$TBB_PREFIX/include" "-L$LLVM_PREFIX/lib" "-L$TBB_PREFIX/lib" .\kaleid.o .\lexer.o .\parser.o -ltbb $($(& "$LLVM_PREFIX/bin/llvm-config.exe" --libs).Split() | % {"-l" + $_.Split(".")[0] }) -D_MD -D_DEBUG -D_DLL -lmsvcrtd -lmsvcprtd
}

link
