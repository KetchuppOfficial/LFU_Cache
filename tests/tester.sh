#!/bin/bash

green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

common_test_dir="../../../tests/"
test_dir="tests/"
ans_dir="answers"
res_dir="results"

function Build
{
    cmake -B build
    cd build
    cmake --build .
}

function Mkdir
{
    rm -rf $1
    mkdir $1
}

function Test ()
{
    local fast="${1}"
    local naive="${1}_naive"
    
    echo "Building naive version..."
    cd naive
    Build
    echo -en "\n"

    echo "Generating answers..."
    for ((i = 0; i < ${n_tests}; i++))
    do
        ./$naive < ${common_test_dir}${test_dir}test_${i}.txt > ${common_test_dir}${ans_dir}_$1/answer_${i}.txt
    done
    echo -en "\n"
    
    cd ../..

    echo "Building fast version..."
    cd fast
    Build
    echo -en "\n"

    echo "Testing..."
    for ((i = 0; i < ${n_tests}; i++))
    do
        ./$fast < ${common_test_dir}${test_dir}test_${i}.txt > ${common_test_dir}${res_dir}_$1/result_${i}.txt

        echo -n "Test $((i + 1)): "
        if diff -Z ${common_test_dir}${ans_dir}_$1/answer_${i}.txt ${common_test_dir}${res_dir}_$1/result_${i}.txt > /dev/null
        then
            echo -e "${green}passed${default}"
        else
            echo -e "${red}failed${default}"
        fi
    done

    cd ../../../tests
}

if [ $# -ne 2 ]
then
    echo "Testing script requires exactly 2 argument"
else
    if [ $2 -le 0 ]
    then
        echo "The number of tests has to be at least 1"
    else
        mode=$1

        if [ $mode = "lfu" ] || [ $mode = "belady" ]; then

            Mkdir ${test_dir}

            echo "Generating tests..."
            n_tests=$2
            python3 test_generator.py ${n_tests} ${test_dir}
            echo -en "\n"
            
            Mkdir "${ans_dir}_${mode}"
            Mkdir "${res_dir}_${mode}"
            
            cd "../$mode"
            Test $mode

        else
            "There is no testing mode with name $2"
        fi
    fi
fi
