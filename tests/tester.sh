#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Testing script requires only 1 argument (number of tests)"
else
    n_files=$1
    
    test_dir="tests"
    ans_dir="answers"
    
    rm -rf ${test_dir}
    rm -rf ${ans_dir}
    
    echo "Generatin tests..."
    mkdir ${test_dir}
    python3 test_generator.py ${n_files} ${test_dir}

    echo "Building naive LFU..."
    g++ ../src/lfu_naive.cpp -I../include -o lfu_naive

    echo "Generating answers..."
    mkdir ${ans_dir}
    for ((i = 0; i < $1; i++))
    do
        ./lfu_naive ./${test_dir}/test_${i}.txt ./${ans_dir}/answer_${i}.txt
    done

    # 4) Remove lfu_naive
    echo "Removing temporary files..."
    rm lfu_naive
fi
