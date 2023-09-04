#!/bin/bash

green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

test_dir="data/"

function Mkdir
{
    rm -rf $1
    mkdir $1
}

function Run_Tests
{
    local fast="../build/tests/${cache}"
    local naive="../build/tests/${cache}_naive"

    local ans_dir="answers_${cache}/"
    local res_dir="results_${cache}/"

    echo "Generating answers..."
    for ((i = 0; i < $n_tests; i++))
    do
        $naive < ${test_dir}test_${i}.txt > ${ans_dir}/answer_${i}.txt
    done
    echo -en "\n"

    echo "Testing..."
    for ((i = 0; i < $n_tests; i++))
    do
        $fast < ${test_dir}test_${i}.txt > ${res_dir}/result_${i}.txt

        echo -n "Test $((i + 1)): "
        if diff -Z ${ans_dir}/answer_${i}.txt ${res_dir}/result_${i}.txt > /dev/null
        then
            echo -e "${green}passed${default}"
        else
            echo -e "${red}failed${default}"
        fi
    done
}

if [ $# -ne 2 ]
then
    echo "Testing script requires exactly 2 arguments"
else
    if [ $2 -le 0 ]
    then
        echo "The number of tests has to be at least 1"
    else
        cache=$1

        if [ $cache = "lfu" ] || [ $cache = "belady" ]
        then
            Mkdir $test_dir

            echo "Generating tests..."
            n_tests=$2
            python3 test_generator.py $n_tests $test_dir
            echo -en "\n"

            Mkdir answers_${cache}
            Mkdir results_${cache}

            echo "Building ${cache} cache..."
            cmake .. -B ../build
            cmake --build ../build --target ${cache}
            cmake --build ../build --target ${cache}_naive
            echo -en "\n"

            Run_Tests

        else
            echo "There is no testing mode with name $2"
        fi
    fi
fi
