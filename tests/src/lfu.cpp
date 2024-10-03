#include <cstddef>
#include <iostream>
#include <exception>

#ifdef NAIVE
#include "lfu_naive.hpp"
#else
#include "lfu.hpp"
#endif

int main() try
{
    using key_type = int;
    using page_type = key_type;

    std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    std::size_t cache_size, n_keys;
    std::cin >> cache_size >> n_keys;

    auto slow_get_page = [](key_type key){ return key; };

    using cache_type =
    #ifdef NAIVE
    yLab::LFU_Naive<key_type, page_type>;
    #else
    yLab::LFU<key_type, page_type>;
    #endif

    cache_type cache{cache_size, slow_get_page};

    auto n_hits = 0uz;
    for (auto key_i = 0uz; key_i != n_keys; ++key_i)
    {
        key_type key;
        std::cin >> key;
        n_hits += cache.lookup_update(key);
    }

    std::cout << n_hits << std::endl;

    return 0;
}
catch (const std::exception &e)
{
    std::cerr << "Caught an instance of " << typeid(e).name()
              << "\nwhat(): " << e.what() << '\n';
    return 1;
}
catch (...)
{
    std::cerr << "Caught an unknown exception\n";
    return 1;
}
