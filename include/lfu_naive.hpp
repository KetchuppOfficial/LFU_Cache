#ifndef INCLUDE_LFU_NAIVE_HPP
#define INCLUDE_LFU_NAIVE_HPP

#include <cstddef>
#include <functional>
#include <vector>
#include <iterator>
#include <algorithm>

namespace yLab
{

template<typename Key_T, typename Page_T>
class LFU_Naive final
{
public:

    using key_type = Key_T;
    using page_type = Page_T;
    using size_type = std::size_t;
    using page_getter = std::function<page_type (const key_type&)>;

private:

    struct Node
    {
        key_type key;
        page_type page;
        size_type counter;
    };

    size_type capacity_;
    std::vector<Node> cache_;
    page_getter slow_get_page_;

public:

    LFU_Naive(size_type capacity, page_getter slow_get_page)
        : capacity_{capacity}, slow_get_page_{slow_get_page}
    {
        cache_.reserve(capacity);
    }

    size_type size() const noexcept { return cache_.size(); }

    bool is_full() const noexcept { return size() == capacity_; }

    bool lookup_update(const key_type &key)
    {
        auto hit = std::ranges::find(cache_, key, &Node::key);

        if (hit == cache_.end())
        {
            if (is_full())
                cache_.erase(std::ranges::min_element(cache_, std::ranges::less{}, &Node::counter));

            cache_.emplace_back(key, slow_get_page_(key), 1);

            return false;
        }
        else
        {
            hit->counter++;
            std::rotate(hit, std::next(hit), cache_.end());

            return true;
        }
    }
};

} // namespace yLab

#endif // INCLUDE_LFU_NAIVE_HPP
