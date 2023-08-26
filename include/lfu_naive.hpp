#ifndef INCLUDE_LFU_NAIVE_HPP
#define INCLUDE_LFU_NAIVE_HPP

#include <vector>
#include <iterator>
#include <algorithm>

namespace yLab
{

template<typename Page_T, typename Key_T = int>
class LFU_Naive final
{
public:

    using size_type = std::size_t;
    using key_type = Key_T;
    using page_getter = std::function<Page_T(const key_type&)>;

private:

    struct Node
    {
        Page_T page_;
        key_type key_;
        int counter_;
    };

    size_type capacity_;
    std::vector<Node> cache_;
    page_getter slow_get_page_;

public:

    LFU_Naive (size_type capacity, page_getter slow_get_page)
              : capacity_{capacity}, slow_get_page_{slow_get_page} {}

    size_type size () const { return cache_.size(); }

    bool is_full () const { return (size() == capacity_); }

    bool lookup_update (const key_type &key)
    {
        auto hit = find_by_key (key);

        if (hit == cache_.end())
        {
            if (is_full())
                cache_.erase (find_min_freq());

            cache_.emplace_back (slow_get_page_(key), key, 1);

            return false;
        }
        else
        {
            hit->counter_++;
            std::rotate (hit, std::next (hit), cache_.end());

            return true;
        }
    }

private:

    using iterator = typename decltype(cache_)::iterator;

    iterator find_by_key (const key_type &key)
    {
        return std::find_if (cache_.begin(), cache_.end(),
                             [&key](auto &&node){ return node.key_ == key; });
    }

    iterator find_min_freq ()
    {
        return std::min_element (cache_.begin(), cache_.end(),
                                 [](auto &&x, auto &&y){ return x.counter_ < y.counter_; });
    }
};

} // namespace yLab

#endif // INCLUDE_LFU_NAIVE_HPP
