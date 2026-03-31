#!/bin/bash
if ! ./build/heapTest > /dev/null;
    then
        result=1
        echo "./build/heapTest - failed!"
    fi