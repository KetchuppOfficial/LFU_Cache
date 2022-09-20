#ifndef LFU_HPP
#define LFU_HPP

#include <list>
#include <unordered_map>
#include <iterator>

namespace Caches
{

template <typename Page_T, typename Key_T = int> class LFU
{   
    size_t capacity_;
    size_t size_ = 0;
    
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

    LFU (size_t capacity) : capacity_{capacity} {}

    size_t size () const { return size_; }

    bool is_full () const { return (size_ == capacity_); }

    template <typename F> bool lookup_update (const Key_T key, const F &slow_get_page)
    {
        auto hit = hash_table_.find (key);

        if (hit == hash_table_.end ())
        {
            request_page (key, slow_get_page);
            return false;
        }
        else
        {
            access_cached (hit);
            return true;
        }
    }

private:

    template <typename F> void request_page (const Key_T key, const F &slow_get_page)
    {
        auto lfu_freq_node = freq_list_.begin ();

        if (is_full ())
        {
            auto lfu_node = lfu_freq_node->node_list.begin ();
            hash_table_.erase (lfu_node->key);
            lfu_freq_node->node_list.erase (lfu_node);
            size_--;
        }

        if (lfu_freq_node->counter != 1)
            lfu_freq_node = freq_list_.insert (lfu_freq_node, {1});

        auto &lfu_list = lfu_freq_node->node_list;

        lfu_list.push_back ({slow_get_page (key), key, lfu_freq_node});
        hash_table_[key] = std::prev (lfu_list.end ());
        size_++;
    }

    void access_cached (const Hash_Iter hit)
    {
        auto page      = hit->second;
        auto freq      = page->parent;
        auto next_freq = std::next (freq);

        if (next_freq == freq_list_.end () || next_freq->counter != freq->counter + 1)
            next_freq = freq_list_.insert (next_freq, {freq->counter + 1});

        auto &next_list = next_freq->node_list;
        next_list.splice (next_list.end (), freq->node_list, page);
        page->parent = next_freq;

        if (freq->node_list.size () == 0)
            freq_list_.erase (freq);
    }
};

} // namespace Caches

#endif // LFU_HPP
