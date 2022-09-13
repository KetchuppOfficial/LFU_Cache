#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Testing script requires only 1 argument (number of tests)"
else
    if [ $1 -le 0 ]
    then
        echo "The number of tests has to be at least 1"
    else
        n_files=$1

        test_dir="tests"
        ans_dir="answers"
        
        rm -rf ${test_dir}
        rm -rf ${ans_dir}
        
        echo "Generating tests..."
        mkdir ${test_dir}
        python3 test_generator.py ${n_files} ${test_dir}

        echo "Building naive LFU..."
        cmake ../lfu_naive -B ../lfu_naive/build
        cmake --build ../lfu_naive/build

        echo "Generating answers..."
        mkdir ${ans_dir}
        cd ../lfu_naive/build
        for ((i = 0; i < $1; i++))
        do
            ./LFU_Naive ../../tests/${test_dir}/test_${i}.txt ../../tests/${ans_dir}/answer_${i}.txt
        done
    fi
fi
