#!/bin/bash

green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

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

if [ $# -ne 1 ]
then
    echo "Testing script requires only 1 argument (number of tests)"
else
    if [ $1 -le 0 ]
    then
        echo "The number of tests has to be at least 1"
    else
        n_tests=$1

        test_dir="tests"
        ans_dir="answers"
        res_dir="results"
        
        Mkdir ${test_dir}
        Mkdir ${ans_dir}
        Mkdir ../lfu/${res_dir}
        
        echo "Generating tests..."
        python3 test_generator.py ${n_tests} ${test_dir}
        echo -en "\n"

        echo "Building naive LFU..."
        cd ../lfu_naive
        Build
        echo -en "\n"

        echo "Generating answers..."
        for ((i = 0; i < ${n_tests}; i++))
        do
            ./LFU_Naive < ../../tests/${test_dir}/test_${i}.txt > ../../tests/${ans_dir}/answer_${i}.txt
        done
        echo -en "\n"

        echo "Building O(1) LFU..."
        cd ../../lfu
        Build
        echo -en "\n"

        echo "Testing..."
        for ((i = 0; i < ${n_tests}; i++))
        do
            ./LFU_Cache < ../../tests/${test_dir}/test_${i}.txt > ../${res_dir}/result_${i}.txt

            echo -n "Test $((i + 1)): "
            if diff -Z ../../tests/${ans_dir}/answer_${i}.txt ../${res_dir}/result_${i}.txt > /dev/null
            then
                echo -e "${green}passed${default}"
            else
                echo -e "${red}failed${default}"
            fi
        done
    fi
fi
