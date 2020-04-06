#!/bin/bash
compile() {
    tname=$1
    ../src/build/nlex --relocation-model pic -o build/$tname.o sources/$tname.nlex && \
        cc -o build/$tname build/$tname.o -lstdc++
}

record() {
    for tpath in "${@}"; do
        tname=$(basename $tpath)
        tname=${tname%.nlex}
        tinput=inputs/$tname.input
        $EDITOR $tinput
        compile $tname
        build/$tname < $tinput > outputs/$tname.stdout 2> outputs/$tname.stderr
        echo "$tname" >> list-tests
    done
}

record "${@}"

swap () {
	local TEMP=tmp.$$
	mv -i $1 "$TEMP"
	mv -i $2 $1
	mv -i "$TEMP" $2
}

cat list-tests | sort -n | uniq > list-tests.new
swap list-tests{,.new}
rm list-tests.new