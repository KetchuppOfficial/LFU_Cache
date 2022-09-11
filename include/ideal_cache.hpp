#ifndef IDEAL_CACHE_HPP
#define IDEAL_CACHE_HPP

#include <list>
#include <unordered_map>
#include <iterator>
#include <iostream>
#include <vector>

namespace Caches
{

template <typename Page_T, typename Key_T = int> class Ideal_Cache
{
    std::size_t capacity_;
    std::size_t size_  = 0;
    std::size_t page_i = 0;

    struct Page_Node
    {
        Page_T page;
        Key_T key;
        std::size_t next_app;
    };

    using Page_Iter = typename std::list<Page_Node>::iterator;

    std::list<Page_Node> page_list_;
    std::unordered_map<Key_T, Page_Iter> hash_table_;
    std::vector<std::pair<Key_T, int>> data_;

    const int no_next_ = -1;

public:

    Ideal_Cache (std::size_t capacity, std::size_t n_keys) : capacity_{capacity}
    {
        for (auto key_i = 0; key_i < n_keys; key_i++)
        {
            Key_T key {};
            std::cin >> key;
            
            std::pair<Key_T, int> pair {key, no_next_};
            data_.push_back(pair);
        }

        for (auto key_i = 0; key_i < n_keys; key_i++)
        {
            for (auto next = key_i + 1; next < n_keys; next++)
            {
                if (data_[key_i].first == data_[next].first)
                {
                    data_[key_i].second = next;
                    break;
                }       
            }
        }
    }

    std::size_t size () const { return size_; }

    bool is_full () const { return (size_ == capacity_); }

    template <typename F> bool lookup_update (const Key_T key, const F &slow_get_page)
    {
        auto hit = hash_table_.find (key);

        if (hit == hash_table_.end ())
        {
            if (is_full ())
            {
                auto farthest      = 0;
                auto node_iter     = page_list_.begin();
                auto farthest_iter = node_iter;
                for (; node_iter != page_list_.end(); node_iter++)
                {
                    if (node_iter->next_app == no_next_)
                    {
                        farthest_iter = node_iter;
                        break;
                    }
                        
                    auto next_app = node_iter->next_app;
                    if (next_app > farthest)
                    {
                        farthest_iter = node_iter;
                        farthest = next_app;
                    }
                }

                hash_table_.erase (farthest_iter->key);
                page_list_.erase (farthest_iter);
                
                size_--;
            }

            page_list_.push_back ({slow_get_page (key), key, data_[page_i++].second});
            hash_table_[key] = std::prev (page_list_.end ());

            size_++;

            return false;
        }
        else
            return true;
    }
};

} // namespace Caches

#endif // IDEAL_CACHE_HPP
