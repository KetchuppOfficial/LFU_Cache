#include <iostream>
#include <cassert>

#include "belady_naive.hpp"

int main ()
{
    using key_type = int;
    
    std::size_t cache_size;
    std::cin >> cache_size;
    assert (std::cin.good());

    std::size_t n_keys;
    std::cin >> n_keys;
    assert (std::cin.good());

    std::vector<key_type> keys_arr;
    keys_arr.reserve (n_keys);
    for (auto key_i = 0; key_i < n_keys; key_i++)
    {
        key_type key;
        std::cin >> key;
        assert (std::cin.good());

        keys_arr.emplace_back (key);
    }

    yLab::Ideal_Cache_Naive cache {cache_size, keys_arr.begin (), keys_arr.end ()};

    auto n_hits  = 0;
    for (auto key_i = 0; key_i < n_keys; key_i++)
        n_hits += cache.lookup_update([](int key){ return key; });
    
    std::cout << n_hits << std::endl;

    return 0;
}
