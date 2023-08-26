#ifndef INCLUDE_LFU_HPP
#define INCLUDE_LFU_HPP

#include <list>
#include <unordered_map>
#include <iterator>

namespace yLab
{

template<typename Page_T, typename Key_T = int>
class LFU final
{
    using key_type = Key_T;

    struct Freq_Node;

    struct Page_Node
    {
        Page_T page_;
        key_type key_;
        typename std::list<Freq_Node>::iterator parent_;
    };

    struct Freq_Node
    {
        int counter_;
        std::list<Page_Node> node_list_;
    };

    using size_type = std::size_t;
    using page_iterator = typename std::list<Page_Node>::iterator;
    using hash_iterator = typename std::unordered_map<key_type, page_iterator>::iterator;

    size_type capacity_;
    std::list<Freq_Node> freq_list_;
    std::unordered_map<key_type, page_iterator> hash_table_;

public:

    explicit LFU (size_type capacity) : capacity_{capacity} {}

    size_type size () const { return hash_table_.size(); }

    bool is_full () const { return (size() == capacity_); }

    template<typename F>
    bool lookup_update (const key_type &key, F slow_get_page)
    {
        auto hit = hash_table_.find (key);

        if (hit == hash_table_.end())
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

    template<typename F>
    void request_page (const key_type &key, F slow_get_page)
    {
        auto lfu_freq_node = freq_list_.begin();

        if (is_full())
        {
            auto lfu_node = lfu_freq_node->node_list_.begin();
            hash_table_.erase (lfu_node->key_);
            lfu_freq_node->node_list_.erase (lfu_node);
        }

        if (lfu_freq_node->counter_ != 1)
            lfu_freq_node = freq_list_.emplace (lfu_freq_node, 1);

        auto &lfu_list = lfu_freq_node->node_list_;

        lfu_list.emplace_back (slow_get_page (key), key, lfu_freq_node);
        hash_table_[key] = std::prev (lfu_list.end());
    }

    void access_cached (const hash_iterator hit)
    {
        auto page_it = hit->second;
        auto freq = page_it->parent_;
        auto next_freq = std::next (freq);

        if (next_freq == freq_list_.end() || next_freq->counter_ != freq->counter_ + 1)
            next_freq = freq_list_.emplace (next_freq, freq->counter_ + 1);

        auto &next_list = next_freq->node_list_;
        next_list.splice (next_list.end(), freq->node_list_, page_it);
        page_it->parent_ = next_freq;

        if (freq->node_list_.empty())
            freq_list_.erase (freq);
    }
};

} // namespace yLab

#endif // INCLUDE_LFU_HPP
