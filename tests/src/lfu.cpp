#include <iostream>
#include <stdexcept>

#ifdef NAIVE
#include "lfu_naive.hpp"
#else
#include "lfu.hpp"
#endif

int main ()
{
    using key_type = int;

    std::size_t cache_size;
    std::cin >> cache_size;
    if (!std::cin.good())
        throw std::runtime_error{"Error while reading the size of cache"};

    std::size_t n_keys;
    std::cin >> n_keys;
    if (!std::cin.good())
        throw std::runtime_error{"Error while reading the number of keys"};

    #ifdef NAIVE
    yLab::LFU_Naive<key_type> cache{cache_size};
    #else
    yLab::LFU<key_type> cache{cache_size};
    #endif

    auto n_hits = 0;
    for (auto key_i = 0; key_i != n_keys; ++key_i)
    {
        key_type key;
        std::cin >> key;
        if (!std::cin.good())
            throw std::runtime_error{"Error while reading a key"};

        n_hits += cache.lookup_update(key, [](key_type key){ return key; });
    }

    std::cout << n_hits << std::endl;

    return 0;
}
