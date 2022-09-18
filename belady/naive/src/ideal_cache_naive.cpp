#include <iostream>
#include <cassert>
#include "belady_naive.hpp"

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

    std::vector<int> keys_arr;
    for (auto key_i = 0; key_i < n_keys; key_i++)
    {
        int key{};
        std::cin >> key;
        assert (std::cin.good ());

        keys_arr.push_back (key);
    }

    Caches::Ideal_Cache_Naive<int> cache {cache_size, keys_arr.begin (), keys_arr.end ()};

    int n_hits {};
    for (auto key_i = 0; key_i < n_keys; key_i++)
        n_hits += cache.lookup_update(Caches::slow_get_page);
    
    std::cout << n_hits << std::endl;

    return 0;
}
