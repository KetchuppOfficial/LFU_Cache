#ifndef BELADY_INCLUDE_BELADY_NAIVE_HPP
#define BELADY_INCLUDE_BELADY_NAIVE_HPP

#include <vector>
#include <iterator>

namespace Caches
{

constexpr int no_next = -1;

using Vector_Iter = typename std::vector<int>::iterator;

template <typename Page_T, typename Key_T = int, typename Keys_Iter = Vector_Iter>
class Ideal_Cache_Naive
{
    size_t capacity_;

    using Node             = typename std::pair<Page_T, Key_T>;
    using Cache_Iter       = typename std::vector<Node>::iterator;
    using Const_Cache_Iter = typename std::vector<Node>::const_iterator;

    std::vector<Node> cache_;
    Keys_Iter input_iter_;
    Keys_Iter end_;

public:

    Ideal_Cache_Naive (size_t capacity, Keys_Iter begin, Keys_Iter end)
                      : capacity_{capacity}, input_iter_{begin}, end_{end} {}

    size_t size () const { return cache_.size (); }

    bool is_full () const { return (size () == capacity_); }

    template <typename F> bool lookup_update (F slow_get_page)
    {
        Key_T key {*input_iter_++};

        auto hit = find_by_key (key);

        if (hit == cache_.end ())
        {           
            const auto input_next_occurrence = find_next_occurence (key);

            if (input_next_occurrence != no_next)
            {
                if (is_full ())
                {
                    const auto [iter, next_occurrence] = find_key_with_latest_occurrence ();

                    if (next_occurrence != no_next &&
                        next_occurrence < input_next_occurrence)
                    {
                        return false;
                    }
                    else
                        cache_.erase (iter);
                }

                cache_.push_back ({slow_get_page (key), key});
            }
            
            return false;
        }
        else
            return true;
    }

private:

    auto find_by_key (const Key_T key) const
    {
        for (auto iter = cache_.begin (), end_iter = cache_.end (); iter != end_iter ; ++iter)
        {
            if (iter->second == key)
                return iter;
        }

        return cache_.end ();
    }

    int find_next_occurence (const Key_T key) const
    {      
        for (auto iter = input_iter_; iter != end_; ++iter)
        {
            if (*iter == key)
                return 1 + static_cast<int>(std::distance (input_iter_, iter));
        }

        return no_next;
    }

    std::pair <Const_Cache_Iter, int> find_key_with_latest_occurrence () const
    {
        auto latest_occurrence = 0;
        auto node_iter         = cache_.begin ();
        auto latest_iter       = node_iter;
        
        for (auto end_iter = cache_.end (); node_iter != end_iter; ++node_iter)
        {
            auto next_occurence = find_next_occurence (node_iter->second);

            if (next_occurence == no_next)
                return {node_iter, next_occurence};
            else if (next_occurence > latest_occurrence)
            {
                latest_iter       = node_iter;
                latest_occurrence = next_occurence;
            }
        }

        return {latest_iter, latest_occurrence};
    }
};

}

#endif // BELADY_INCLUDE_BELADY_NAIVE_HPP
