#ifndef INCLUDE_LFU_HPP
#define INCLUDE_LFU_HPP

#include <list>
#include <unordered_map>
#include <iterator>
#include <functional>

namespace yLab
{

template<typename Page_T, typename Key_T = int>
class LFU final
{
public:

    using key_type = Key_T;
    using size_type = std::size_t;
    using page_getter = std::function<Page_T(const Key_T&)>;

private:

    struct Freq_Node;

    using freq_iterator = typename std::list<Freq_Node>::iterator;

    struct Page_Node
    {
        Page_T page_;
        key_type key_;
        freq_iterator parent_;
    };

    struct Freq_Node
    {
        int counter_;
        std::list<Page_Node> node_list_;
    };

    using page_iterator = typename std::list<Page_Node>::iterator;
    using hash_iterator = typename std::unordered_map<key_type, page_iterator>::iterator;

    size_type capacity_;
    std::list<Freq_Node> freq_list_;
    std::unordered_map<key_type, page_iterator> hash_table_;
    page_getter slow_get_page_;

public:

    LFU (size_type capacity, page_getter slow_get_page)
        : capacity_{capacity}, slow_get_page_{slow_get_page} {}

    size_type size () const { return hash_table_.size(); }

    bool is_full () const { return (size() == capacity_); }

    bool lookup_update (const key_type &key)
    {
        hash_iterator hit = hash_table_.find (key);

        if (hit == hash_table_.end())
        {
            request_page (key);
            return false;
        }
        else
        {
            access_cached (hit);
            return true;
        }
    }

private:

    void request_page (const key_type &key)
    {
        freq_iterator lfu_freq_node_it = freq_list_.begin();

        if (is_full())
        {
            auto lfu_node = lfu_freq_node_it->node_list_.begin();
            hash_table_.erase (lfu_node->key_);
            lfu_freq_node_it->node_list_.erase (lfu_node);
        }

        if (lfu_freq_node_it->counter_ != 1)
            lfu_freq_node_it = freq_list_.emplace (lfu_freq_node_it, 1);

        auto &lfu_list = lfu_freq_node_it->node_list_;

        lfu_list.emplace_back (slow_get_page_(key), key, lfu_freq_node_it);
        hash_table_[key] = std::prev (lfu_list.end());
    }

    void access_cached (hash_iterator hit)
    {
        page_iterator page_it = hit->second;
        freq_iterator freq = page_it->parent_;
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
