#!/bin/bash
# This script is run from root project directory

result=0
for file_in in $(find tests/CSVPrettyPrinter -name "*.csv");
do
    file_out="${file_in%*.csv}_out"
    if ! ./build/CSVPrettyPrinter ${file_in} tempfile;
    then
        result=1
        echo "./build/CSVPrettyPrinter ${file_in} [output] - failed!"
    fi
    if ! diff $file_out tempfile;
    then
        result=1
        echo "Testcase \"${file_out}\" - failed!"
    fi
done

exit ${result}