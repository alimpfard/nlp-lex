#!/bin/bash

compile() {
    tname=$1
    ../src/build/nlex --relocation-model pic -o build/$tname.o sources/$tname.nlex >/dev/null 2>&1 && \
        cc -o build/$tname build/$tname.o -lstdc++
}

assertEqual() {
    in0=$2
    in1=$3
    cmp <(sed -e 's/res at.*$//g' $in0) <(sed -e 's/res at.*$//g' $in1)
}

runtest() {
    tname=$1
    printf "Testing $tname: "
    build/$tname < $tinput > output.stdout 2> output.stderr || echo "exec FAIL" && \
    assertEqual stdout output.stdout outputs/$tname.stdout || echo "stdout FAIL" && \
    assertEqual stderr output.stderr outputs/$tname.stderr || echo "stderr FAIL" && \
    echo "ok"
}

for test in `cat list-tests`; do
    tname=$test

    if [ -e inputs/$tname.input ]
    then
        tinput=inputs/$tname.input
    else
        tinput=<(echo -e "\n")
    fi

    compile $test &&\
    runtest $test
done
