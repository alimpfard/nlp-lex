gen() {
  (
    cat $1
    echo ".nlvm"
    echo ".end"
  ) | ../a.out 2> ll.ll
}

paste() {
  llvm-link ll.ll test.ll > all.bc
}

compile() {
  opt all.bc -o all-opt.bc -da -constmerge -gvn -globaldce -instcombine -lcssa -licm -adce -constprop -inline -tailcallelim -instcombine -adce -dce -sink -simplifycfg
  llc -filetype=obj -relocation-model=pic all-opt.bc
  gcc all-opt.o
}

all() {
  gen
  paste
  compile
}

gen $@
paste; compile
