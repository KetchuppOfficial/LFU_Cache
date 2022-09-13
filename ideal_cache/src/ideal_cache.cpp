#include <iostream>
#include <cassert>
#include "ideal_cache.hpp"

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

    Caches::Ideal_Cache<int> cache {cache_size, n_keys};

    int n_hits {};
    for (auto key_i = 0; key_i < n_keys; key_i++)
        n_hits += cache.lookup_update(Caches::slow_get_page);
    
    std::cout << n_hits << std::endl;

    return 0;
}
