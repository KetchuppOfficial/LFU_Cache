#ifndef BELADY_INCLUDE_BELADY_NAIVE_HPP
#define BELADY_INCLUDE_BELADY_NAIVE_HPP

#include <vector>
#include <iterator>

namespace yLab
{

template<typename Key_Iter, typename Page_T = int>
class Ideal_Cache_Naive final
{
    using size_type = std::size_t;
    using key_iterator = Key_Iter;
    using key_type = typename std::iterator_traits<key_iterator>::value_type;
    using node_type = typename std::pair<Page_T, key_type>;
    using const_iterator = typename std::vector<node_type>::const_iterator;
    
    size_type capacity_;
    std::vector<node_type> cache_;
    key_iterator input_iter_;
    key_iterator end_;

public:

    Ideal_Cache_Naive (size_type capacity, key_iterator begin, key_iterator end)
                      : capacity_{capacity}, input_iter_{begin}, end_{end} {}

    size_type size () const { return cache_.size(); }

    bool is_full () const { return (size() == capacity_); }

    bool contains (const key_type &key) const { return find_by_key (key) != cache_.end(); }

    template <typename F>
    bool lookup_update (F slow_get_page)
    {
        auto key {*input_iter_++};

        if (contains (key))
            return true;
        else
        {           
            auto input_next_occurrence = find_next_occurence (key);

            if (input_next_occurrence != no_next)
            {
                if (is_full())
                {
                    auto [iter, next_occurrence] = find_key_with_latest_occurrence();

                    if (next_occurrence != no_next && next_occurrence < input_next_occurrence)
                        return false;
                    else
                        cache_.erase (iter);
                }

                cache_.emplace_back (slow_get_page (key), key);
            }
            
            return false;
        }
    }

private:

    static constexpr int no_next = -1;

    const_iterator find_by_key (const key_type &key) const
    {
        return std::find_if (cache_.begin(), cache_.end(),
                             [&key](auto &&node){ return node.second == key; });
    }

    int find_next_occurence (const key_type &key) const
    {      
        auto it = std::find_if (input_iter_, end_, [&key](auto &&elem){ return elem == key; });
        if (it == end_)
            return no_next;
        else
            return 1 + static_cast<int>(std::distance (input_iter_, it));
    }

    std::pair<const_iterator, int> find_key_with_latest_occurrence () const
    {
        auto latest_occurrence = 0;
        auto node_iter = cache_.begin();
        auto latest_iter = node_iter;
        
        for (auto end_iter = cache_.end(); node_iter != end_iter; ++node_iter)
        {
            auto next_occurence = find_next_occurence (node_iter->second);

            if (next_occurence == no_next)
                return std::pair{node_iter, next_occurence};
            else if (next_occurence > latest_occurrence)
            {
                latest_iter       = node_iter;
                latest_occurrence = next_occurence;
            }
        }

        return std::pair{latest_iter, latest_occurrence};
    }
};

} // namespace yLab

#endif // BELADY_INCLUDE_BELADY_NAIVE_HPP
