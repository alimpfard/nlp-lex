all: rts

lexer:
	g++ -ggdb3 -Wmaybe-uninitialized -Wuninitialized -Wall -Wno-narrowing -Wno-write-strings kaleid.cpp lexer.cc parser.cc `llvm-config --cxxflags --ldflags --system-libs --libs core` -std=c++17 -ltcl -pthread -fopenmp -ltbb -fexceptions

format:
	clang-format -i *.{hpp,cc,tcc,cpp}

rts:
	clang -S -static -O3 -g -emit-llvm rts.c
	llvm-as rts.ll
	mv rts.bc test.bc
	xxd -i test.bc > test_bc.h
	
