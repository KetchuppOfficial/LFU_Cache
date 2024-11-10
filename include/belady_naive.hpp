#ifndef INCLUDE_BELADY_NAIVE_HPP
#define INCLUDE_BELADY_NAIVE_HPP

#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>

namespace yLab
{

template<typename Key_T, typename Page_T>
class Ideal_Cache_Naive final
{
    using size_type = std::size_t;
    using key_type = Key_T;
    using page_type = Page_T;
    using node_type = typename std::pair<key_type, page_type>;
    using const_iterator = typename std::vector<node_type>::const_iterator;
    using page_getter = std::function<page_type (const key_type&)>;

    std::vector<node_type> cache_;
    std::deque<key_type> input_;
    size_type capacity_;
    page_getter slow_get_page_;

public:

    template<std::forward_iterator It>
    requires std::convertible_to<typename std::iterator_traits<It>::value_type, key_type>
    Ideal_Cache_Naive(size_type capacity, page_getter slow_get_page, It begin, It end)
        : input_(begin, end), capacity_{capacity}, slow_get_page_{slow_get_page}
    {
        cache_.reserve(capacity);
    }

    size_type size() const noexcept { return cache_.size(); }

    bool is_full() const noexcept { return size() == capacity_; }

    bool contains(const key_type &key) const { return find_by_key(key) != cache_.end(); }

    bool lookup_update()
    {
        if (input_.empty())
            return false;

        const auto key = std::move(input_.front());
        input_.pop_front();

        if (contains(key))
            return true;

        auto input_next_occurrence = find_next_occurrence(key);

        if (input_next_occurrence != no_next)
        {
            if (is_full())
            {
                auto [iter, next_occurrence] = find_key_with_latest_occurrence();

                if (next_occurrence != no_next && next_occurrence < input_next_occurrence)
                    return false;
                else
                    cache_.erase(iter);
            }

            cache_.emplace_back(key, slow_get_page_(key));
        }

        return false;
    }

private:

    static constexpr int no_next = -1;

    const_iterator find_by_key(const key_type &key) const
    {
        return std::ranges::find(cache_, key, &node_type::first);
    }

    int find_next_occurrence(const key_type &key) const
    {
        auto it = std::ranges::find(input_, key);
        if (it == input_.end())
            return no_next;
        else
            return 1 + static_cast<int>(std::distance(input_.begin(), it));
    }

    std::pair<const_iterator, int> find_key_with_latest_occurrence() const
    {
        int latest_occurrence = 0;
        auto node_iter = cache_.begin();
        auto latest_iter = node_iter;

        for (auto end_iter = cache_.end(); node_iter != end_iter; ++node_iter)
        {
            int next_occurrence = find_next_occurrence(node_iter->second);

            if (next_occurrence == no_next)
                return std::pair{node_iter, next_occurrence};
            else if (next_occurrence > latest_occurrence)
            {
                latest_iter = node_iter;
                latest_occurrence = next_occurrence;
            }
        }

        return std::pair{latest_iter, latest_occurrence};
    }
};

} // namespace yLab

#endif // INCLUDE_BELADY_NAIVE_HPP
