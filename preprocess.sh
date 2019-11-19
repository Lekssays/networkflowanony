#!/bin/bash

if [ "$1" != "" ]; then
    echo "Parse network flows"
    time python3 parser.py -f $1;
    sleep 3;
    echo "Map IPs to Integers"
    time python3 mapper.py -f $1;
    sleep 3;
    echo "Build Anonymized Graph"
    g++ builder.cc;
    sleep 1;
    a='id_mapping_';
    b=$1;
    output=${a}${b};
    time ./a.out < $output;
else
    echo "No arguments";
fi