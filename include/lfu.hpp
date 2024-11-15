#ifndef INCLUDE_LFU_HPP
#define INCLUDE_LFU_HPP

#include <cstddef>
#include <functional>
#include <list>
#include <unordered_map>
#include <iterator>

namespace yLab
{

template<typename Key_T, typename Page_T>
class LFU final
{
public:

    using key_type = Key_T;
    using page_type = Page_T;
    using size_type = std::size_t;
    using page_getter = std::function<page_type (const key_type&)>;

private:

    struct Freq_Node;

    using freq_iterator = typename std::list<Freq_Node>::iterator;

    struct Page_Node
    {
        key_type key;
        page_type page;
        freq_iterator parent;
    };

    struct Freq_Node
    {
        std::list<Page_Node> node_list;
        int counter;

        explicit Freq_Node(int c) : counter{c} {}
    };

    using page_iterator = typename std::list<Page_Node>::iterator;
    using hash_iterator = typename std::unordered_map<key_type, page_iterator>::iterator;

    size_type capacity_;
    std::list<Freq_Node> freq_list_;
    std::unordered_map<key_type, page_iterator> hash_table_;
    page_getter slow_get_page_;

public:

    LFU(size_type capacity, page_getter slow_get_page)
        : capacity_{capacity}, hash_table_(capacity), slow_get_page_{slow_get_page} {}

    size_type size() const noexcept { return hash_table_.size(); }

    bool is_full() const noexcept { return size() == capacity_; }

    bool lookup_update(const key_type &key)
    {
        hash_iterator hit = hash_table_.find(key);

        if (hit == hash_table_.end())
        {
            request_page(key);
            return false;
        }
        else
        {
            access_cached(hit);
            return true;
        }
    }

private:

    void request_page(const key_type &key)
    {
        freq_iterator lfu_freq_node_it = freq_list_.begin();

        if (is_full())
        {
            auto &lfu_list = lfu_freq_node_it->node_list;
            auto lfu_node_it = lfu_list.begin();

            hash_table_.erase(lfu_node_it->key);
            lfu_list.erase(lfu_node_it);
        }

        if (lfu_freq_node_it == freq_list_.end() || lfu_freq_node_it->counter != 1)
            lfu_freq_node_it = freq_list_.emplace(lfu_freq_node_it, 1);

        auto &lfu_list = lfu_freq_node_it->node_list;

        lfu_list.emplace_back(key, slow_get_page_(key), lfu_freq_node_it);
        hash_table_[key] = std::prev(lfu_list.end());
    }

    void access_cached(hash_iterator hit)
    {
        page_iterator page_it = hit->second;
        freq_iterator freq_it = page_it->parent;

        auto next_freq = std::next(freq_it);
        if (next_freq == freq_list_.end() || next_freq->counter != freq_it->counter + 1)
            next_freq = freq_list_.emplace(next_freq, freq_it->counter + 1);

        auto &next_list = next_freq->node_list;
        next_list.splice(next_list.end(), freq_it->node_list, page_it);
        page_it->parent = next_freq;

        if (freq_it->node_list.empty())
            freq_list_.erase(freq_it);
    }
};

} // namespace yLab

#endif // INCLUDE_LFU_HPP
