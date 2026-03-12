#!/bin/bash
if ! ./build/graphTest > /dev/null;
    then
        result=1
        echo "./build/graphTest - failed!"
    fi