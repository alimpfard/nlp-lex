all: lexer rts


lexer:
	g++ -ggdb3 -Wmaybe-uninitialized -Wuninitialized -Wall -Wno-narrowing -Wno-write-strings kaleid.cpp lexer.cc parser.cc `llvm-config --cxxflags --ldflags --system-libs --libs core` -std=c++17 -ltcl -pthread -fopenmp -ltbb -fexceptions

format:
	clang-format -i *.{hpp,cc,tcc,cpp}

rts:
	clang++ -O3 -S -emit-llvm wordtree_link.cc
	mv wordtree_link.ll playground
