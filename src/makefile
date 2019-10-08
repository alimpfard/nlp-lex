all: lexer


lexer:
	g++ -ggdb3 -std=c++17 lexer.cc parser.cc -I/usr/include  -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/lib -lLLVM-8

format:
	clang-format -i *.{hpp,cc,tcc}
