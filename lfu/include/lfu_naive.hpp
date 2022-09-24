#ifndef LFU_INCLUDE_LFU_NAIVE_HPP
#define LFU_INCLUDE_LFU_NAIVE_HPP

#include <vector>
#include <iterator>
#include <algorithm>

namespace Caches
{

template <typename Page_T, typename Key_T = int> class LFU_Naive
{
    size_t capacity_;

    struct Node
    {
        Page_T page_;
        Key_T key_;
        int counter_;
    };

    std::vector<Node> cache_;

    using Cache_Iter = typename std::vector<Node>::iterator;
    using Const_Cache_Iter = typename std::vector<Node>::const_iterator;

public:

    LFU_Naive (size_t capacity) : capacity_{capacity} {}

    size_t size () const { return cache_.size(); }

    bool is_full () const { return (size() == capacity_); }

    bool lookup_update (const Key_T key, Page_T (* slow_get_page) (Key_T))
    {
        auto hit = find_by_key (key);
        
        if (hit == cache_.end ())
        {
            if (is_full ())
            {
                auto min_freq_iter = find_min_freq ();
                cache_.erase (min_freq_iter);
            }

            cache_.push_back ({slow_get_page (key), key, 1});

            return false;
        } 
        else
        {
            hit->counter_++;
            std::rotate (hit, hit + 1, cache_.end ());
            return true;
        }
    }

private:

    Cache_Iter find_by_key (const Key_T key)
    {
        for (auto iter = cache_.begin (), end_iter = cache_.end (); iter != end_iter; ++iter)
        {
            if (iter->key_ == key)
                return iter;
        }

        return cache_.end (); 
    }

    Const_Cache_Iter find_min_freq () const
    {
        auto iter          = cache_.begin ();
        auto min_freq_iter = iter;
        auto min_freq      = iter->counter_;

        for (auto end_iter = cache_.end (); iter != end_iter; ++iter)
        {
            if (iter->counter_ < min_freq)
            {
                min_freq_iter = iter;
                min_freq      = iter->counter_;
            }
        }
        
        return min_freq_iter;
    }
};

}

#endif // LFU_INCLUDE_LFU_NAIVE_HPP
