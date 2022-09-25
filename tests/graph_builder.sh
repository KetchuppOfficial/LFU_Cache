#!/bin/bash

test_dir="tests/"

function Mkdir
{
    rm -rf $1
    mkdir $1
}

function Run_Cache ()
{
    local cache=$1
    local fast="../build/${cache}/fast/${cache}"
    local data_dir="${cache}_data/"

    Mkdir $data_dir

    for ((i = 0; i < $n_tests; i++))
    do
        $fast < ${test_dir}test_${i}.txt > ${data_dir}result_${i}.txt
    done
}

if [ $# -ne 1 ]
then
    echo "Graph builer requires exactly 1 argument"
else
    if [ $1 -le 0 ]
    then
        echo "The number of tests has to be at least 1"
    else
        Mkdir $test_dir

        echo "Generating tests..."
        n_tests=$1
        python3 test_generator.py $n_tests $test_dir
        echo -en "\n"

        echo "Building caches..."
        cmake .. -B ../build
        cmake --build ../build --target lfu
        cmake --build ../build --target belady
        echo -en "\n"

        echo "Running LFU cache..."
        Run_Cache "lfu"
        echo -en "\n"

        echo "Running Belady's cache..."
        Run_Cache "belady"
        echo -en "\n"

        echo "Building the bar chart..."
        python3 bar_chart.py $n_tests
    fi
fi
