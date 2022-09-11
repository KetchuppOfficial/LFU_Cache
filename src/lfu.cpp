#include <iostream>
#include "lfu.hpp"

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

    int n_hits {};
    for (;;)
    {
        int page {};
        std::cin >> page;

        if (std::cin.eof())
            break;

        n_hits += cache.lookup_update(page, Caches::slow_get_page);
    }

    std::cout << "n_hits = " << n_hits << std::endl;
    
    return 0;
}
