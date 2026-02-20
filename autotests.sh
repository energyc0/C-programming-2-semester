#!/bin/bash

if ! cmake -B build > /dev/null ||  ! cmake --build build > /dev/null;
then
    echo "Failed to build the project!"
    exit 1
fi


result=0

for script in $(find ./tests -name "*.sh")
do
    if ! ${script};
    then
        echo "${script} - failed!"
        result=1
    fi
done

rm -r build

if [ ${result} == "0" ];
then
    echo "Tests passed."
else
    echo "Tests failed."
fi
exit ${result}