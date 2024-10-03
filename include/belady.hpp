#ifndef INCLUDE_BELADY_HPP
#define INCLUDE_BELADY_HPP

#include <unordered_map>
#include <deque>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <list>
#include <concepts>
#include <functional>

namespace yLab
{

template<typename Key_T>
class Occurrence_Table final
{
    std::unordered_map<Key_T, std::deque<int>> table_;

public:

    static constexpr int no_next = -1;

    template<typename key_iterator>
    Occurrence_Table(key_iterator begin, key_iterator end)
    {
        std::for_each(begin, end,
                      [this, index = 0](const Key_T &key) mutable { add(key, index++); });

        for (auto &bucket : table_)
            bucket.second.pop_front();
    }

    void add(const Key_T &key, int occurrence) { table_[key].push_back(occurrence); }

    void pop_first(const Key_T &key) { table_.at(key).pop_front(); }

    int first(const Key_T &key) const
    {
        const auto &deque = table_.at(key);
        return deque.empty() ? no_next : deque.front();
    }
};

template<typename Key_T, typename Page_T>
class Ideal_Cache final
{
    using size_type = std::size_t;
    using key_type = Key_T;
    using page_type = Page_T;
    using page_node = typename std::pair<key_type, page_type>;
    using page_iterator = typename std::list<page_node>::iterator;
    using const_page_iterator = typename std::list<page_node>::const_iterator;
    using page_getter = std::function<page_type (const key_type&)>;

    std::list<page_node> page_list_;
    std::unordered_map<key_type, page_iterator> hash_table_;

    std::deque<key_type> input_;
    Occurrence_Table<key_type> occurrence_table_;

    size_type capacity_;
    page_getter slow_get_page_;

public:

    template<std::forward_iterator It>
    requires std::convertible_to<typename std::iterator_traits<It>::value_type, key_type>
    Ideal_Cache(std::size_t capacity, page_getter slow_get_page, It begin, It end)
        : input_(begin, end), occurrence_table_{begin, end}, capacity_{capacity},
          slow_get_page_{slow_get_page} {}

    size_type size() const noexcept { return page_list_.size(); }

    bool is_full() const noexcept { return size() == capacity_; }

    bool lookup_update()
    {
        if (input_.empty())
            return false;

        const auto key = std::move(input_.front());
        input_.pop_front();

        auto hit = hash_table_.find(key);

        if (hit != hash_table_.end())
        {
            occurrence_table_.pop_first(key);
            return true;
        }

        const int input_next_occurrence = occurrence_table_.first(key);

        if (input_next_occurrence != Occurrence_Table<key_type>::no_next)
        {
            if (is_full())
            {
                auto [iter, next_occurrence] = find_key_with_latest_occurrence();

                if (next_occurrence != Occurrence_Table<key_type>::no_next &&
                    next_occurrence < input_next_occurrence)
                {
                    occurrence_table_.pop_first(key);
                    return false;
                }
                else
                {
                    auto &key_to_erase = iter->first;

                    hash_table_.erase(key_to_erase);
                    occurrence_table_.pop_first(key_to_erase);
                    page_list_.erase(iter);
                }
            }

            page_list_.emplace_back(key, slow_get_page_(key));
            hash_table_[key] = std::prev(page_list_.end());
        }

        return false;
    }

private:

    std::pair<const_page_iterator, int> find_key_with_latest_occurrence() const
    {
        int latest_occurrence = 0;
        auto node_iter = page_list_.begin();
        auto latest_iter = node_iter;

        for (auto end_iter = page_list_.end(); node_iter != end_iter; ++node_iter)
        {
            auto next_occurrence = occurrence_table_.first(node_iter->first);

            if (next_occurrence == Occurrence_Table<key_type>::no_next)
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

#endif // INCLUDE_BELADY_HPP
