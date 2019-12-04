# will add tagger back once we have an idea of how
# to do it in the first place
# all: compiler tagger

all: compiler

compiler:
	make -C src

tagger:
	cd postagger/nlp-tools && \
	rm -rf build && \
	mkdir build && \
	cd build && \
	cmake .. && \
	make && \
	cp tool_train ../../../tools && \
	cp libpredict.a ../../../src/playground && \
	cp libpost.a ../../../src/playground
