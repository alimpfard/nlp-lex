gen() {
  (cat $1; echo ".nlvm"; echo ".end") | ../a.out 2> ll.ll
}

paste() {
  cat ll.ll test.ll > all.ll
}

compile() {
  opt all.ll -o all.bc -da -constmerge -gvn -globaldce -instcombine -lcssa -licm -adce -constprop -inline
  llc -filetype=obj -relocation-model=pic all.bc
  gcc all.o
}

all() {
  gen
  paste
  compile
}

gen $@
paste; compile
