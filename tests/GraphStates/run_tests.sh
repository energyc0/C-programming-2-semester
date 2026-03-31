#!/bin/bash
# This script is run from root project directory

result=0
for file_in in $(find tests/GraphStates -name "test*.txt");
do
    file_res="${file_in%*.txt}_res"
    if ! ./build/graphStates ${file_in} > tempfile;
    then
        result=1
        echo "./build/graphStates ${file_in} - failed!"
    fi
    if ! diff $file_res tempfile;
    then
        result=1
        echo "Testcase \"${file_res}\" - failed!"
    fi
done

rm -f tempfile
exit ${result}