#ifndef BELADY_HPP
#define BELADY_HPP

#include <list>
#include <unordered_map>
#include <iterator>
#include <deque>
#include <vector>
#include <algorithm>

namespace Caches
{

constexpr int no_next = -1;

template <typename Key_T> class Occurrence_Table
{
    std::unordered_map<Key_T, std::deque<int>> table_;
    
public:
    template <typename Keys_Iter> Occurrence_Table (Keys_Iter begin, Keys_Iter end) : table_{}
    {
        std::for_each (begin, end, [this, index = 0](const Key_T &key) mutable { add (key, index); index++; });

        for (auto &bucket : table_)
        {
            auto &deque = bucket.second;
            if (!deque.empty ())
                deque.pop_front ();
        }
    }

    void add (const Key_T key, const int occurrence) { table_[key].push_back (occurrence); }

    void pop_first (const Key_T key) { table_.find (key)->second.pop_front (); }

    int first (const Key_T key) const
    {
        auto &deque = table_.find (key)->second;
        return (deque.empty ()) ? no_next : deque.front ();
    }
};

using Vector_Iter = typename std::vector<int>::iterator;

template <typename Page_T, typename Key_T = int, typename Keys_Iter = Vector_Iter> 
class Ideal_Cache
{
    size_t capacity_;

    using Page_Node       = typename std::pair<Page_T, Key_T>;
    using Page_Iter       = typename std::list<Page_Node>::iterator;
    using Const_Page_Iter = typename std::list<Page_Node>::const_iterator;

    std::list<Page_Node> page_list_;
    std::unordered_map<Key_T, Page_Iter> hash_table_;

    Occurrence_Table<Key_T> occurrence_table_;

    Keys_Iter input_iter_;

public:

    Ideal_Cache (size_t capacity, Keys_Iter begin, Keys_Iter end)
                : capacity_{capacity}, occurrence_table_{begin, end}, input_iter_{begin} {}

    size_t size () const { return page_list_.size (); }

    bool is_full () const { return (size () == capacity_); }

    bool lookup_update (Page_T (* slow_get_page) (Key_T))
    {
        Key_T key {*input_iter_++};

        auto hit = hash_table_.find (key);

        if (hit == hash_table_.end ())
        {
            auto input_next_occurrence = occurrence_table_.first (key);

            if (input_next_occurrence != no_next)
            {
                if (is_full ())
                {               
                    const auto [iter, next_occurrence] = find_key_with_latest_occurrence ();

                    if (next_occurrence != no_next &&
                        next_occurrence < input_next_occurrence)
                    {
                        occurrence_table_.pop_first (key);
                        return false;
                    }
                    else
                    {
                        auto key_to_erase = iter->second;

                        hash_table_.erase (key_to_erase);
                        occurrence_table_.pop_first (key_to_erase);
                        page_list_.erase (iter);
                    }
                }

                page_list_.push_back ({slow_get_page (key), key});
                hash_table_[key] = std::prev (page_list_.end ());
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

    std::pair<Const_Page_Iter, int> find_key_with_latest_occurrence () const
    {
        auto latest_occurrence = 0;
        auto node_iter         = page_list_.begin ();
        auto latest_iter       = node_iter;

        for (auto end_iter = page_list_.end (); node_iter != end_iter; ++node_iter)
        {
            auto next_occurence = occurrence_table_.first (node_iter->second);

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

} // namespace Caches

#endif // BELADY_HPP
