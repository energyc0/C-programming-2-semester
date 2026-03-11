#!/bin/bash
if ! ./build/AVLTreeTests > /dev/null;
    then
        result=1
        echo "./build/AVLTreeTests - failed!"
    fi