# HWC - caches

This project is the first task on course "Uses and applications of the C++ language" by [K.I.Vladimirov](https://github.com/tilir). The task was to implement one of suggested cache replacement algorithms. I chose LFU cache.

## Project structure

4 cache algorithms were implemented: [naive](./lfu/include/lfu_naive.hpp) and [O(1)](./lfu/include/lfu.hpp) LFU, [naive](./belady/include/belady_naive.hpp) and [O(n)](./belady/include/belady.hpp) Belady's algorithm.

## How to install
```bash
git clone git@github.com:KetchuppOfficial/LFU_Cache.git
cd LFU_Cache
```

## Requirements

The following applications have to be installed:

1) CMake of version 3.16 (or higher)

2) clang++

## How to build

### 0) Make sure you are in the root directory of the project (i.e. LFU_Cache/)

### 1) Build desired cache(s)

```bash
cmake -B build
cmake --build build [--target <tgt>]
```
**tgt** can be **lfu**, **lfu_naive**, **belady** or **belady_naive**.
If --target option is omitted, all 4 caches will be built.

### 3) Install cache(s)
```bash
cmake --install build [--prefix <prefix>] [--component <comp>]
```
**prefix** can be any directory you want to install the cache(s) in.
If --prefix option is omitted, executable files will be installed in ./bin/ directory.

**comp** can be **lfu**, **lfu_naive**, **belady** or **belady_naive**.
If --component option is omitted, all 4 caches will be installed.

## How to generate tests

If you want to run some tests, go to [./tests](./tests/) directory.

Let **N** be the number of tests you want to run, then command sequence:
```bash
./tester.sh mode N
```
generates **N** tests, builds naive cache and runs it on the tests to compute answers. After that fast cache is run on those tests to compute its own answers. Finally, both sets of answers are compared. If answers on the same test differ between naive and fast implementations, then this test is considered "failed". It is considered "passed" otherwise. **mode** argument has to be of one of 2 values only: **lfu** or **belady**. It specifies what algorithm will be tested.

## Comparison of algorithms

If you want to build a bar chart that compares output results of two algorithms, go to [./tests](./tests/) directory.

Let **N** be the number of tests, results on which you want to represented in a bar chart, then command sequence:
```bash
./graph_builder.sh N
```
generates **N** tests, builds fast caches and runs them of those tests. After that a python script builds a bar chart based on the results of each cache.

Here is an example of a bar chart:
![bar_char](./tests/algorithm_comparison.png)
