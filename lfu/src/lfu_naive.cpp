#include <iostream>
#include <cassert>

#include "lfu_naive.hpp"

int main ()
{   
    using key_type = int;
    
    std::size_t cache_size;
    std::cin >> cache_size;
    assert (std::cin.good());

    std::size_t n_keys;
    std::cin >> n_keys;
    assert (std::cin.good());

    yLab::LFU_Naive<key_type> cache{cache_size};

    auto n_hits = 0;
    for (auto key_i = 0; key_i < n_keys; key_i++)
    {
        key_type key;
        std::cin >> key;
        assert (std::cin.good());

        n_hits += cache.lookup_update(key, [](int key){ return key; });
    }

    std::cout << n_hits << std::endl;
    
    return 0;
}
