#!/bin/bash

if [ "$1" != "" ]; then
    time python3 networkPreprocessor.py -f $1;
    sleep 3;
    time python3 mapper.py -f $1;
    sleep 3;
    g++ cycleDFS.cc;
    sleep 1;
    a='id_mapping_';
    b=$1;
    output=${a}${b};
    time ./a.out < $output;
else
    echo "No arguments";
fi