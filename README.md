# HWC - caches

This project is the first task on course "Uses and applications of the C++ language" by [K.I.Vladimirov](https://github.com/tilir). The task was to implement one of suggested cache replacement algorithms. I chose LFU cache.

## Project structure

3 cache algorithms were implemented: [O(1) LFU](./include/lfu.hpp), [naive LFU](./include/lfu_naive.hpp) and [ideal cache](./include/ideal_cache.hpp) (Belady's algorithm).

## How to install
```bash
git clone git@github.com:KetchuppOfficial/LFU_Cache.git
cd LFU_Cache
```

## How to generate tests

Let (N) be the number of tests you want to run, then command sequence:
```bash
cd tests
./tester.sh N
```
generates **N** tests, builds naive LFU cache and runs it on the tests to compute answers. After that O(1) LFU cache is run on those tests to compute its own answers. Finally, both sets of answers are compared. If any O(1)-answer differs from that of the naive implementation, then the test is considered "failed". It is considered "passed" otherwise.
