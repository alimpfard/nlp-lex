TBB_PREFIX = F:\Pwogwam\TBB
LLVM_PREFIX = F:\Pwogwam\LLVM
SHELL := powershell

kaleid.o:
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs\
		-I${LLVM_PREFIX}/include -I${TBB_PREFIX}/include\
		-L${LLVM_PREFIX}/lib -L${TBB_PREFIX}/lib\
		kaleid.cc\
		D_MD -D_DEBUG -D_DLL -c -g

lexer.o:
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs\
		-I${LLVM_PREFIX}/include -I${TBB_PREFIX}/include\
		-L${LLVM_PREFIX}/lib -L${TBB_PREFIX}/lib\
		lexer.cc\
		D_MD -D_DEBUG -D_DLL -c -g

parser.o:
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs\
		-I${LLVM_PREFIX}/include -I${TBB_PREFIX}/include\
		-L${LLVM_PREFIX}/lib -L${TBB_PREFIX}/lib\
		parser.cc\
		D_MD -D_DEBUG -D_DLL -c -g

link: kaleid.o lexer.o parser.o
	clang++ -std=c++17 -Wno-narrowing -Wwrite-strings -Wno-non-pod-varargs\
		-I${LLVM_PREFIX}\include -I${TBB_PREFIX}/include -L${LLVM_PREFIX}\lib\
		-L${TBB_PREFIX}\lib\
		.\kaleid.o .\lexer.o .\parser.o\
		-ltbb\
		$$($$(${LLVM_PREFIX}\bin\llvm-config.exe --libs).Split() | % {"-l" + $_.Split(".")[0] })\
		-D_MD -D_DEBUG -D_DLL -lmsvcrtd -lmsvcprtd

compile: kaleid.o lexer.o parser.o

ALL: link
