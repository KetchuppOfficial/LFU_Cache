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

    return 0;
}
