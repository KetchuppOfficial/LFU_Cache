#ifndef BELADY_INCLUDE_BELADY_HPP
#define BELADY_INCLUDE_BELADY_HPP

#include <list>
#include <unordered_map>
#include <iterator>
#include <deque>
#include <vector>
#include <algorithm>
#include <utility>

namespace yLab
{

constexpr int no_next = -1;

template<typename Key_T>
class Occurrence_Table final
{
    std::unordered_map<Key_T, std::deque<int>> table_;
    
public:

    template<typename key_iterator>
    Occurrence_Table (key_iterator begin, key_iterator end) : table_{}
    {
        std::for_each (begin, end, [this, index = 0](const Key_T &key) mutable
                                   { add (key, index); index++; });

        for (auto &bucket : table_)
        {
            auto &deque = bucket.second;
            if (!deque.empty())
                deque.pop_front();
        }
    }

    void add (const Key_T &key, int occurrence) { table_[key].push_back (occurrence); }

    void pop_first (const Key_T &key) { table_.find (key)->second.pop_front(); }

    int first (const Key_T &key) const
    {
        auto &deque = table_.find (key)->second;
        return (deque.empty()) ? no_next : deque.front();
    }
};

template<typename Key_Iter, typename Page_T = int>
class Ideal_Cache final
{
    using size_type = std::size_t;
    using key_iterator = Key_Iter;
    using key_type = typename std::iterator_traits<key_iterator>::value_type;
    using page_node = typename std::pair<Page_T, key_type>;
    using page_iterator = typename std::list<page_node>::iterator;
    using const_page_iterator = typename std::list<page_node>::const_iterator;

    size_type capacity_;

    std::list<page_node> page_list_;
    std::unordered_map<key_type, page_iterator> hash_table_;

    Occurrence_Table<key_type> occurrence_table_;

    key_iterator input_iter_;

public:

    Ideal_Cache (size_type capacity, key_iterator begin, key_iterator end)
                : capacity_{capacity}, occurrence_table_{begin, end}, input_iter_{begin} {}

    size_type size () const { return page_list_.size(); }

    bool is_full () const { return (size() == capacity_); }

    template<typename F>
    bool lookup_update (F slow_get_page)
    {
        auto key {*input_iter_++};

        auto hit = hash_table_.find (key);

        if (hit == hash_table_.end())
        {
            auto input_next_occurrence = occurrence_table_.first (key);

            if (input_next_occurrence != no_next)
            {
                if (is_full())
                {               
                    auto [iter, next_occurrence] = find_key_with_latest_occurrence ();

                    if (next_occurrence != no_next &&
                        next_occurrence < input_next_occurrence)
                    {
                        occurrence_table_.pop_first (key);
                        return false;
                    }
                    else
                    {
                        auto &key_to_erase = iter->second;

                        hash_table_.erase (key_to_erase);
                        occurrence_table_.pop_first (key_to_erase);
                        page_list_.erase (iter);
                    }
                }

                page_list_.emplace_back (slow_get_page (key), key);
                hash_table_[key] = std::prev (page_list_.end());
            }

            return false;
        }
        else
        {
            occurrence_table_.pop_first (key);
            return true;
        }       
    }

private:

    std::pair<const_page_iterator, int> find_key_with_latest_occurrence () const
    {
        auto latest_occurrence = 0;
        auto node_iter         = page_list_.begin();
        auto latest_iter       = node_iter;

        for (auto end_iter = page_list_.end(); node_iter != end_iter; ++node_iter)
        {
            auto next_occurence = occurrence_table_.first (node_iter->second);

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

#endif // BELADY_INCLUDE_BELADY_HPP
