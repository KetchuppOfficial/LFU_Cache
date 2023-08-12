#include <iostream>
#include <iterator>
#include <stdexcept>

#ifdef NAIVE
#include "belady_naive.hpp"
#else
#include "belady.hpp"
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

    std::vector<key_type> keys_arr;
    keys_arr.reserve (n_keys);
    for (auto key_i = 0; key_i != n_keys; ++key_i)
    {
        key_type key;
        std::cin >> key;
        if (!std::cin.good())
            throw std::runtime_error{"Error while reading a key"};

        keys_arr.emplace_back (key);
    }

    #ifdef NAIVE
    yLab::Ideal_Cache_Naive cache{cache_size, keys_arr.begin(), keys_arr.end()};
    #else
    yLab::Ideal_Cache cache{cache_size, keys_arr.begin(), keys_arr.end()};
    #endif

    auto n_hits = 0;
    for (auto key_i = 0; key_i != n_keys; ++key_i)
        n_hits += cache.lookup_update([](key_type key){ return key; });

    std::cout << n_hits << std::endl;

    return 0;
}
