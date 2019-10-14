gen() {
  (cat $1; echo ".nlvm"; echo ".end") | ../a.out 2> ll.ll
}

paste() {
  cat ll.ll test.ll > all.ll
}

compile() {
  llc -filetype=obj -relocation-model=pic all.ll
  gcc all.o
}

all() {
  gen
  paste
  compile
}

gen $@; paste; compile
