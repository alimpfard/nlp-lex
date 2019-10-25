gen() {
  (
    cat $1
    echo ".dot"
    echo ".gengraph"
    echo ".nlvm"
    echo ".end"
  ) | ../a.out 2> ll.ll
}

paste() {
  llvm-link test.ll ll.ll > all.bc
  # cat ll.ll test.ll > all.ll
  # opt all.ll -o all.bc
}

compile() {
  # opt all.bc -o all-opt.bc -da -constmerge -gvn -globaldce -instcombine -lcssa -licm -adce -constprop -inline -tailcallelim -instcombine -adce -dce -sink -simplifycfg
  opt all.bc -o all-opt.bc -instcombine -dce -constprop -inline -tailcallelim -instcombine -dce -sink -simplifycfg
  # cp all.bc all-opt.bc
  llc -filetype=obj -relocation-model=pic all-opt.bc
  gcc all-opt.o
  gcc all-opt.o -shared -o all.so
}

all() {
  gen "$@"
  paste
  compile
}

gen "$@"
paste; compile
