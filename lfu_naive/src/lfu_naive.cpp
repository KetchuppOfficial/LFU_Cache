#include "lfu_naive.hpp"

#include <iostream>
#include <fstream>
#include <cassert>

namespace Caches
{

int slow_get_page (int key) { return key; }

}

int main (int argc, char *argv[])
{
    assert (argc == 3);

    const char *input_name  = argv[1];
    const char *output_name = argv[2];

    std::ifstream input {input_name, std::ios::in};
    
    std::size_t cache_size {};
    input >> cache_size;
    assert (input.good ());

    std::size_t n_keys {};
    input >> n_keys;
    assert (input.good ());

    Caches::LFU_Naive<int> cache {cache_size};

    int n_hits {};
    for (auto key_i = 0; key_i < n_keys; key_i++)
    {
        int key {};
        input >> key;
        assert (!input.eof ());

        n_hits += cache.lookup_update(key, Caches::slow_get_page);
    }

    std::ofstream output {output_name, std::ios::out};
    output << n_hits << "\n";
    
    return 0;
}
