#ifndef LFU_HPP
#define LFU_HPP

#include <list>
#include <unordered_map>
#include <iterator>

namespace Caches
{

template <typename Page_T, typename Key_T = int> class LFU
{   
    std::size_t size_;
    
    struct Freq_Node;
    using Freq_Iter = typename std::list<Freq_Node>::iterator;

    struct Page_Node
    {
        Page_T page;
        Key_T key;
        Freq_Iter parent;
    };

    using Page_Iter = typename std::list<Page_Node>::iterator;

    struct Freq_Node
    {
        int counter;
        std::list<Page_Node> node_list;
    };

    std::list<Freq_Node> freq_list_;
    std::unordered_map<Key_T, Page_Iter> hash_table_;

    using Hash_Iter = typename std::unordered_map<Key_T, Page_Iter>::iterator;
    
public:

    LFU (std::size_t size) : size_{size} {}

    std::size_t size () const { return freq_list_.size (); }

    bool is_full () const { return (size() == size_); }

    template <typename F> auto lookup_update (const Key_T key, const F &slow_get_page)
    {
        auto hit = hash_table_.find (key);

        if (hit == hash_table_.end ())
            return request_page (key, slow_get_page)->page;
        else
            return access_cached (hit)->page;
    }

    template <typename F> auto lookup_update (const Key_T key, const F &slow_get_page, bool &is_hit)
    {
        auto hit = hash_table_.find (key);

        if (hit == hash_table_.end ())
        {
            is_hit = false;
            return request_page (key, slow_get_page)->page;
        }
        else
        {
            is_hit = true;
            return access_cached (hit)->page;
        }
    }

private:

    template <typename F> auto request_page (const Key_T key, const F &slow_get_page)
    {
        auto lfu_freq_node = freq_list_.begin ();

        if (is_full())
        {
            auto lfu_node = lfu_freq_node->node_list.begin ();
            hash_table_.erase (lfu_node->key);
            lfu_freq_node->node_list.erase (lfu_node);
        }

        if (lfu_freq_node->counter != 1)
            lfu_freq_node = freq_list_.insert (lfu_freq_node, {1});

        auto &lfu_list = lfu_freq_node->node_list;  
        lfu_list.push_front ({slow_get_page (key), key, lfu_freq_node});

        auto new_page = lfu_list.begin ();
        hash_table_[key] = new_page;

        return new_page;
    }

    auto access_cached (const Hash_Iter hit)
    {
        auto page      = hit->second;
        auto freq      = page->parent;
        auto next_freq = std::next (freq);

        if (next_freq == freq_list_.end () || next_freq->counter != freq->counter + 1)
            next_freq = freq_list_.insert (next_freq, {freq->counter});

        auto &next_list = next_freq->node_list;
        next_list.splice (next_list.end (), freq->node_list, page);
        page->parent = next_freq;

        if (freq->node_list.size () == 0)
            freq_list_.erase (freq);

        return page;
    }
};

} // namespace Caches

#endif // LFU_HPP
