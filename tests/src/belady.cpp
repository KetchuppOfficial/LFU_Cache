#include <cstddef>
#include <iostream>
#include <exception>
#include <vector>

#ifdef NAIVE
#include "belady_naive.hpp"
#else
#include "belady.hpp"
#endif

int main() try
{
    using key_type = int;
    using page_type = key_type;

    std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    std::size_t cache_size, n_keys;
    std::cin >> cache_size >> n_keys;

    std::vector<key_type> keys_arr;
    keys_arr.reserve(n_keys);
    for (auto i = 0uz; i != n_keys; ++i)
    {
        key_type key;
        std::cin >> key;
        keys_arr.push_back(key);
    }

    auto slow_get_page = [](key_type key){ return key; };

    using cache_type =
    #ifdef NAIVE
    yLab::Ideal_Cache_Naive<key_type, page_type>;
    #else
    yLab::Ideal_Cache<key_type, page_type>;
    #endif

    cache_type cache{cache_size, slow_get_page, keys_arr.begin(), keys_arr.end()};

    auto n_hits = 0uz;
    for (auto key_i = 0uz; key_i != n_keys; ++key_i)
        n_hits += cache.lookup_update();

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
