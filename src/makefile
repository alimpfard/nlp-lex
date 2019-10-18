all: lexer


lexer:
	g++ -ggdb3 lexer.cc parser.cc `llvm-config --cxxflags --ldflags --system-libs --libs core` -std=c++17 -ltcl -pthread -fopenmp -ltbb -fexceptions

format:
	clang-format -i *.{hpp,cc,tcc}
