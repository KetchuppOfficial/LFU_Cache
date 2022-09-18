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

enum Next
{
    no_next = -1
};

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

        #ifdef DUMP
        // Occurence_Table state dump
        std::cout << "***********************************" << std::endl;
        std::for_each (table_.begin(), table_.end(),
                       [](const std::pair<Key_T, std::deque<int>> &bucket)
                       {
                            auto &deque = bucket.second;
                            std::cout << "KEY = " << bucket.first << "; N_ELEMS = " << deque.size () << std::endl;

                            std::for_each (deque.begin (), deque.end (), 
                                           [](const int &next_occurrence)
                                           {
                                                std::cout << "\tnext_occurrence = " << next_occurrence << std::endl;
                                           });
                       });
        std::cout << "***********************************" << std::endl;
        #endif
    }

    void add (const Key_T key, const int occurrence) { table_[key].push_back (occurrence); }

    void pop_first (const Key_T key) { table_.find (key)->second.pop_front (); }

    int first (const Key_T key) const
    {
        auto &deque = table_.find (key)->second;
        return (deque.empty ()) ? static_cast<int>(no_next) : deque.front ();
    }
};

using Vector_Iter = typename std::vector<int>::iterator;

template <typename Page_T, typename Key_T = int, typename Keys_Iter = Vector_Iter> 
class Ideal_Cache
{
    std::size_t capacity_;

    struct Page_Node
    {
        Page_T page_;
        Key_T key_;
    };

    using Page_Iter = typename std::list<Page_Node>::iterator;
    using Const_Page_Iter = typename std::list<Page_Node>::const_iterator;

    std::list<Page_Node> page_list_;
    std::unordered_map<Key_T, Page_Iter> hash_table_;

    Occurrence_Table<Key_T> occurrence_table_;

    Keys_Iter input_iter_;

public:

    Ideal_Cache (std::size_t capacity, Keys_Iter begin, Keys_Iter end)
                : capacity_{capacity}, occurrence_table_{begin, end}, input_iter_{begin} {}

    std::size_t size () const { return page_list_.size (); }

    bool is_full () const { return (size () == capacity_); }

    template <typename F> bool lookup_update (const F &slow_get_page)
    {
        Key_T key {*input_iter_++};

        auto hit = hash_table_.find (key);

        if (hit == hash_table_.end ())
        {
            auto input_next_occurrence = occurrence_table_.first (key);

            if (input_next_occurrence != static_cast<int>(no_next))
            {
                if (is_full ())
                {               
                    const auto latest = find_key_with_latest_occurrence ();

                    if (latest.next_occurrence != static_cast<int>(no_next) &&
                        latest.next_occurrence < input_next_occurrence)
                    {
                        occurrence_table_.pop_first (key);
                        return false;
                    }
                    else
                    {
                        auto key_to_erase = latest.iter->key_;

                        hash_table_.erase (key_to_erase);
                        occurrence_table_.pop_first (key_to_erase);
                        page_list_.erase (latest.iter);
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

    struct Latest
    {
        Const_Page_Iter iter;
        int next_occurrence;
    };

    Latest find_key_with_latest_occurrence () const
    {
        auto latest_occurrence = 0;
        auto node_iter         = page_list_.begin ();
        auto latest_iter       = node_iter;

        for (; node_iter != page_list_.end (); node_iter++)
        {
            auto next_occurence = occurrence_table_.first (node_iter->key_);

            if (next_occurence == static_cast<int>(no_next))
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
