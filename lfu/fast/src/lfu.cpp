#include <iostream>
#include <cassert>
#include "lfu.hpp"

namespace Caches
{

int slow_get_page (int key) { return key; }

}

int main ()
{
    std::size_t cache_size {};
    std::cin >> cache_size;
    assert (std::cin.good ());

    std::size_t n_keys {};
    std::cin >> n_keys;
    assert (std::cin.good ());

    Caches::LFU<int> cache {cache_size};

    int n_hits {};
    for (auto key_i = 0; key_i < n_keys; key_i++)
    {
        int key {};
        std::cin >> key;
        assert (std::cin.good ());

        n_hits += cache.lookup_update(key, Caches::slow_get_page);
    }

    std::cout << n_hits << std::endl;
    
    return 0;
}
