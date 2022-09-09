#include "lfu.hpp"
#include <iostream>
#include <string>
#include <limits>

namespace Caches
{

int slow_get_page (int key)
{
    return key++;
}

}

int main ()
{
    size_t cache_size {};
    std::cin >> cache_size;
    
    Caches::LFU<int> cache {cache_size};

    cache.lookup_update(2, Caches::slow_get_page);
    
    return 0;
}
