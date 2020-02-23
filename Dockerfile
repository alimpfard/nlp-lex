FROM archlinux/base
MAINTAINER Ali Mohammad Pur <devanothertest@gmail.com>

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm llvm openmp intel-tbb tcl bash
RUN pacman -S --noconfirm git cmake make clang
RUN git clone https://github.com/alimpfard/nlp-lex

WORKDIR /nlp-lex/src

RUN mkdir build
RUN cd build && cmake .. && make

WORKDIR /nlp-lex

RUN mkdir -p /nlex/bin

RUN mv src/build/nlex /nlex/bin
RUN mv tools /nlex
RUN mv examples /nlex

WORKDIR /nlex

RUN rm -rf /nlp-lex
RUN pacman -Rcs --noconfirm cmake make git

ENTRYPOINT /bin/bash
