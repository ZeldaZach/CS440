#ifndef ZACH_HALPERN_MAP_HPP
#define ZACH_HALPERN_MAP_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <utility>

namespace cs540
{
    #define MAX_LEVEL_SKIPLIST 30

    template <typename Key_T, typename Mapped_T>
    class Map
    {
        private:
            typedef std::pair<const Key_T, Mapped_T> ValueType;

            // https://www.geeksforgeeks.org/skip-list-set-3-searching-deletion/
            class SkipList
            {
                public:
                    struct Node
                    {
                        // vector[Index] = Value
                        // Index = Level, Value = Node to insert new Node after
                        std::vector<Node *> forward, backward;
                        bool nodeHasValue;

                        Node(int levels, bool initialized = false) : nodeHasValue(initialized)
                        {
                            for (int i = 0; i <= levels; i++)
                            {
                                forward.push_back(nullptr);
                                backward.push_back(nullptr);
                            }
                        }

                        bool hasValue()
                        {
                            return nodeHasValue;
                        }

                        virtual ~Node() = default;
                    };

                    struct NodeValues : public Node
                    {
                        NodeValues(const ValueType vals, int levels) : Node(levels, true), values(vals) {}

                        ValueType& getValues()
                        {
                            return values;
                        }

                        private:
                            ValueType values;
                    };

                    SkipList() : probability(0.5), max_level(MAX_LEVEL_SKIPLIST), current_level(0), unique_nodes(0), head(new Node(max_level)), tail(new Node(max_level))
                    {
                        for (int i = 0; i <= max_level; i++)
                        {
                            head->forward[i] = tail;
                            head->backward[i] = nullptr;

                            tail->backward[i] = head;
                            tail->forward[i] = nullptr;
                        }
                    }

                    ~SkipList()
                    {
                        for (Node *i = head; i != tail; )
                        {
                            if (i)
                            {
                                auto j = i->forward[0];
                                delete i;
                                i = j;
                            }
                        }

                        delete tail;
                    }

                    size_t size()
                    {
                        return unique_nodes;
                    }

                    void insert(ValueType new_val)
                    {
                        // Index = Level, Value = Node to insert after
                        std::vector<Node *> tmp = head->forward;

                        // Start at the head, let it update so we don't re-traverse the list for no reason
                        Node *current = head;
                        for (int lev = current_level; lev >= 0; lev--)
                        {
                            while (current->forward[lev] && current->forward[lev] != tail && static_cast<typename SkipList::NodeValues *>(current->forward[lev])->getValues().first < new_val.first)
                            {
                                current = current->forward[lev];
                            }

                            // Will say which value to insert AFTER
                            tmp[lev] = current;
                        }

                        /*
                         * reached level 0 and next pointer to
                         * right, which is desired position to
                         * insert new key.
                         */
                        current = current->forward[0];
                        if (current == nullptr || current == tail || !(static_cast<typename SkipList::NodeValues *>(current)->getValues().first == new_val.first))
                        {
                            int level_to_insert_at = rand_level();

                            if (level_to_insert_at > current_level)
                            {
                                // If the max level is not defined, set tmp[i] = head because
                                // We want to insert after the head at these levels for the first value
                                for (int i = current_level + 1; i <= level_to_insert_at; i++)
                                {
                                    tmp[i] = head;
                                }
                                current_level = level_to_insert_at;
                            }

                            // create new node with random level generated
                            NodeValues* new_node = new NodeValues(new_val, level_to_insert_at);

                            // insert node by rearranging pointers
                            for (int i = 0; i <= level_to_insert_at; i++)
                            {
                                if (tmp[i] == tail)
                                {
                                    new_node->forward[i] = tail;
                                    new_node->backward[i] = tail->backward[i];
                                    tail->backward[i]->forward[i] = new_node;
                                    tail->backward[i] = new_node;
                                }
                                else if (tmp[i] == head)
                                {
                                    new_node->forward[i] = head->forward[i];
                                    new_node->backward[i] = head;
                                    head->forward[i]->backward[i] = new_node;
                                    head->forward[i] = new_node;
                                }
                                else
                                {
                                    // A <=> C becomes A <=> B <=> C
                                    new_node->forward[i] = tmp[i]->forward[i]; 	//  B -> C
                                    tmp[i]->forward[i]->backward[i] = new_node; // 	B <- C
                                    new_node->backward[i] = tmp[i]; 			// 	A <- B
                                    tmp[i]->forward[i] = new_node; 				// 	A -> B
                                }
                            }

                            unique_nodes++;
                        }
                    }

                    Node* find(const Key_T key)
                    {
                        // Start at the head, let it update so we don't re-traverse the list for no reason
                        Node *current = head;
                        for (int lev = current_level; lev >= 0; lev--)
                        {
                            while (current->forward[lev] != tail && static_cast<typename SkipList::NodeValues *>(current->forward[lev])->getValues().first < key)
                            {
                                current = current->forward[lev];
                            }
                        }

                        current = current->forward[0];

                        // Value is not in list OR place where it should be is not the correct key
                        if (current == nullptr || current == head || current == tail || !(static_cast<typename SkipList::NodeValues *>(current)->getValues().first == key))
                        {
                            return nullptr;
                        }
                        else
                        {
                            return current;
                        }
                    }

                    void erase(const Key_T key)
                    {
                        Node *current = head;
                        std::vector<Node *> tmp = head->forward;

                        for (int lev = current_level; lev >= 0; lev--)
                        {
                            while (current->forward[lev] != tail && static_cast<typename SkipList::NodeValues *>(current->forward[lev])->getValues().first < key)
                            {
                                current = current->forward[lev];
                            }
                            tmp[lev] = current;
                        }

                        current = current->forward[0];
                        if (current != tail && static_cast<typename SkipList::NodeValues *>(current)->getValues().first == key)
                        {
                            for (int i = 0; i <= current_level; i++)
                            {

                                if (tmp[i]->forward[i] != current)
                                {
                                    break;
                                }

                                // A <=> B <=> C becomes A <=> C
                                tmp[i]->forward[i] = current->forward[i]; // 	A -> C
                                current->forward[i]->backward[i] = tmp[i]; //	A <- C
                            }

                            delete current;

                            while (current_level > 0 && head->forward[current_level] == tail)
                            {
                                current_level--;
                            }

                            unique_nodes--;
                        }
                    }

                    int rand_level() const
                    {
                        int v = 0;
                        while ((static_cast<float>(std::rand()) / RAND_MAX) < probability && v < max_level)
                        {
                            v++;
                        }
                        return std::min(v, MAX_LEVEL_SKIPLIST);
                    }

                    Node* head_sentinel()
                    {
                        return head;
                    }

                    Node* tail_sentinel()
                    {
                        return tail;
                    }

                private:
                    const float probability;
                    int max_level, current_level, unique_nodes;
                    Node *head, *tail;
            };

        public:
            class ConstIterator;
            class ReverseIterator;

            class Iterator
            {
                public:
                    typename SkipList::Node *node_ptr;

                    Iterator() = delete;
                    Iterator(typename SkipList::Node *n) : node_ptr(n) {}
                    Iterator(ConstIterator iter) : node_ptr(iter.node_ptr) {}

                    Iterator& operator=(const Iterator &save)
                    {
                        node_ptr = save.node_ptr;
                        return *this;
                    }

                    Iterator &operator++()
                    {
                        node_ptr = node_ptr->forward[0];
                        return *this;
                    }

                    Iterator operator++(int)
                    {
                        auto &i = *this;
                        node_ptr = node_ptr->forward[0];
                        return i;
                    }

                    Iterator &operator--()
                    {
                        node_ptr = node_ptr->backward[0];
                        return *this;
                    }

                    Iterator operator--(int)
                    {
                        auto &i = *this;
                        node_ptr = node_ptr->backward[0];
                        return i;
                    }

                    bool isNull() const
                    {
                        return (node_ptr == nullptr);
                    }

                    ValueType& operator*() const
                    {
                        return static_cast<typename SkipList::NodeValues *>(node_ptr)->getValues();
                    }

                    ValueType* operator->() const
                    {
                        return &(static_cast<typename SkipList::NodeValues *>(node_ptr)->getValues());
                    }

                    friend bool operator==(const Iterator &a, const Iterator &b)
                    {
                        if (!a.isNull() && a.node_ptr->hasValue())
                        {
                            if (!b.isNull() && b.node_ptr->hasValue())
                            {
                                return (
                                        static_cast<typename SkipList::NodeValues *>(a.node_ptr)->getValues()
                                        ==
                                        static_cast<typename SkipList::NodeValues *>(b.node_ptr)->getValues()
                                );
                            }

                            return false;
                        }

                        return a.node_ptr == b.node_ptr;

                    }

                    friend bool operator !=(const Iterator &a, const Iterator &b)
                    {
                        return !(a == b);
                    }

                    friend bool operator !=(const Iterator &a, const ConstIterator &b)
                    {
                        return !(a == b);
                    }

                    friend bool operator==(const Iterator &a, const ConstIterator &b)
                    {
                        if (!a.isNull() && a.node_ptr->hasValue())
                        {
                            if (!b.isNull() && b.node_ptr->hasValue())
                            {
                                return (
                                        static_cast<typename SkipList::NodeValues *>(a.node_ptr)->getValues()
                                        ==
                                        static_cast<typename SkipList::NodeValues *>(b.node_ptr)->getValues()
                                );
                            }

                            return false;
                        }

                        return a.node_ptr == b.node_ptr;
                    }
            };

            class ConstIterator
            {
                public:
                    typename SkipList::Node *node_ptr;

                    ConstIterator() = delete;
                    ConstIterator(typename SkipList::Node *n) : node_ptr(n) {}
                    ConstIterator(Iterator iter) : node_ptr(iter.node_ptr) {}

                    ConstIterator& operator=(const ConstIterator &save)
                    {
                        node_ptr = save.node_ptr;
                        return *this;
                    }

                    ConstIterator &operator++()
                    {
                        node_ptr = node_ptr->forward[0];
                        return *this;
                    }

                    ConstIterator operator++(int)
                    {
                        auto &i = *this;
                        node_ptr = node_ptr->forward[0];
                        return i;
                    }

                    ConstIterator &operator--()
                    {
                        node_ptr = node_ptr->backward[0];
                        return *this;
                    }

                    ConstIterator operator--(int)
                    {
                        auto &i = *this;
                        node_ptr = node_ptr->backward[0];
                        return i;
                    }

                    bool isNull() const
                    {
                        return (node_ptr == nullptr);
                    }

                    const ValueType& operator*() const
                    {
                        return (static_cast<typename SkipList::NodeValues *>(node_ptr)->getValues());
                    }

                    const ValueType* operator->() const
                    {
                        return &(static_cast<typename SkipList::NodeValues *>(node_ptr)->getValues());
                    }

                    friend bool operator==(const ConstIterator &a, const ConstIterator &b)
                    {
                        if (!a.isNull() && a.node_ptr->hasValue())
                        {
                            if (!b.isNull() && b.node_ptr->hasValue())
                            {
                                return (
                                        static_cast<typename SkipList::NodeValues *>(a.node_ptr)->getValues()
                                        ==
                                        static_cast<typename SkipList::NodeValues *>(b.node_ptr)->getValues()
                                );
                            }

                            return false;
                        }

                        return a.node_ptr == b.node_ptr;
                    }

                    friend bool operator==(const ConstIterator &a, const Iterator &b)
                    {
                        if (!a.isNull() && a.node_ptr->hasValue())
                        {
                            if (!b.isNull() && b.node_ptr->hasValue())
                            {
                                return (
                                        static_cast<typename SkipList::NodeValues *>(a.node_ptr)->getValues()
                                        ==
                                        static_cast<typename SkipList::NodeValues *>(b.node_ptr)->getValues()
                                );
                            }

                            return false;
                        }

                        return a.node_ptr == b.node_ptr;
                    }

                    friend bool operator !=(const ConstIterator &a, const ConstIterator &b)
                    {
                        return !(a == b);
                    }

                    friend bool operator !=(const ConstIterator &a, const Iterator &b)
                    {
                        return !(a == b);
                    }
            };

            class ReverseIterator
            {
                public:
                    typename SkipList::Node *node_ptr;

                    ReverseIterator() = delete;

                    ReverseIterator(typename SkipList::Node *n) : node_ptr(n) {}

                    ReverseIterator& operator=(const ReverseIterator &save)
                    {
                        node_ptr = save.node_ptr;
                        return *this;
                    }

                    ReverseIterator &operator++(int)
                    {
                        auto &i = *this;
                        node_ptr = node_ptr->backward[0];
                        return i;

                    }

                    ReverseIterator operator++()
                    {
                        node_ptr = node_ptr->backward[0];
                        return *this;
                    }

                    ReverseIterator &operator--(int)
                    {
                        auto &i = *this;
                        node_ptr = node_ptr->forward[0];
                        return i;
                    }

                    ReverseIterator operator--()
                    {
                        node_ptr = node_ptr->forward[0];
                        return *this;
                    }

                    bool isNull() const
                    {
                        return (node_ptr == nullptr);
                    }

                    ValueType& operator*() const
                    {
                        return (static_cast<typename SkipList::NodeValues *>(node_ptr)->getValues());
                    }

                    ValueType* operator->() const
                    {
                        return &(static_cast<typename SkipList::NodeValues *>(node_ptr)->getValues());
                    }

                    friend bool operator==(const ReverseIterator &a, const ReverseIterator &b)
                    {
                        if (!a.isNull() && a.node_ptr->hasValue())
                        {
                            if (!b.isNull() && b.node_ptr->hasValue())
                            {
                                return (
                                        static_cast<typename SkipList::NodeValues *>(a.node_ptr)->getValues()
                                        ==
                                        static_cast<typename SkipList::NodeValues *>(b.node_ptr)->getValues()
                                );
                            }

                            return false;
                        }

                        return a.node_ptr == b.node_ptr;
                    }

                    friend bool operator !=(const ReverseIterator &a, const ReverseIterator &b)
                    {
                        return !(a == b);
                    }
            };

            Map() : list(new SkipList()) {}

            Map(const Map &old) : list(new SkipList())
            {
                for (auto i = old.begin(); i != old.end(); i++)
                {
                    list->insert((*i));
                }
            }

            Map &operator=(const Map &o)
            {
                for (auto i = o.begin(); i != o.end(); i++)
                {
                    list->insert((*i));
                }
                return *this;
            }

            Map(std::initializer_list<std::pair<const Key_T, Mapped_T> > i_list) : list(new SkipList())
            {
                for (auto val : i_list)
                {
                    insert(val);
                }
            }

            ~Map()
            {
                delete list;
            }

            size_t size() const
            {
                return list->size();
            }

            bool empty() const
            {
                return (size() == 0);
            }

            Iterator begin()
            {
                return Iterator(list->head_sentinel()->forward[0]);
            }

            Iterator end()
            {
                return Iterator(list->tail_sentinel());
            }

            ConstIterator begin() const
            {
                return ConstIterator(list->head_sentinel()->forward[0]);
            }

            ConstIterator end() const
            {
                return ConstIterator(list->tail_sentinel());
            }

            ReverseIterator rbegin()
            {
                return ReverseIterator(list->tail_sentinel()->backward[0]);
            }

            ReverseIterator rend()
            {
                return ReverseIterator(list->head_sentinel());
            }

            Iterator find(const Key_T &key)
            {
                auto val = list->find(key);
                if (val == nullptr)
                {
                    return end();
                }

                return Iterator(val);
            }

            ConstIterator find(const Key_T &key) const
            {
                auto val = list->find(key);
                if (val == nullptr)
                {
                    return end();
                }

                return ConstIterator(val);
            }

            Mapped_T &at(const Key_T &key)
            {
                auto ret = list->find(key); // Node
                if (ret == nullptr)
                {
                    throw std::out_of_range("Key not in range");
                }

                return (static_cast<typename SkipList::NodeValues *>(ret)->getValues().second);
            }

            const Mapped_T &at(const Key_T &key) const
            {
                auto ret = list->find(key);
                if (ret == nullptr)
                {
                    throw std::out_of_range("Key not in range");
                }

                return (static_cast<typename SkipList::NodeValues *>(ret)->getValues().second);
            }

            Mapped_T &operator[](const Key_T &key)
            {
                auto ret = list->find(key); // Node
                if (ret == nullptr)
                {
                    list->insert(std::make_pair(key, Mapped_T()));
                    ret = list->find(key);
                }

                return static_cast<typename SkipList::NodeValues *>(ret)->getValues().second;
            }

            std::pair<Iterator, bool> insert(const ValueType &values)
            {
                Iterator it = find(values.first);
                if (it != end())
                {
                    return std::make_pair(it, false);
                }

                list->insert(values);
                return std::make_pair(find(values.first), true);
            }

            template <typename IT_T>
            void insert(IT_T range_beg, IT_T range_end)
            {
                for (auto i = range_beg; i != range_end; i++)
                {
                    insert(i);
                }
            }

            void erase(Iterator pos)
            {
                if (pos == end())
                {
                    throw std::out_of_range("Out of range");
                }
                list->erase(static_cast<typename SkipList::NodeValues *>(pos.node_ptr)->getValues().first);
            }

            void erase(const Key_T &key)
            {
                auto tmp = list->find(key); // Node
                if (tmp == nullptr)
                {
                    throw std::out_of_range("Out of range");
                }
                list->erase(key);
            }

            void clear()
            {
                delete list;
                list = new SkipList();
            }

            friend bool operator==(const Map<Key_T, Mapped_T> &a, const Map<Key_T, Mapped_T> &b)
            {
                if (a.size() == b.size())
                {
                    auto a_iter = a.begin();
                    auto b_iter = b.begin();

                    while (a_iter != a.end())
                    {
                        if (a_iter != b_iter)
                        {
                            return false;
                        }

                        a_iter++;
                        b_iter++;
                    }

                    return true;
                }

                return false;
            }

            friend bool operator !=(const Map<Key_T, Mapped_T> &a, const Map<Key_T, Mapped_T> &b)
            {
                return !(a == b);
            }

            friend bool operator<(const Map<Key_T, Mapped_T> &a, const Map<Key_T, Mapped_T> &b)
            {
                /*
                A map M1 is less than a map M2 if:
                 - ele in M1 less than correspoding ele in M2
                 - all corresponding eles in both maps are equal, M1 shorter than M2
                */

                auto aIter = a.begin();
                auto bIter = b.begin();

                while (aIter != a.end())
                {
                    if (*aIter.second < *bIter.second)
                    {
                        aIter++;
                        bIter++;
                    }
                    else
                    {
                        return false;
                    }
                }

                return true;
            }

        private:
            Map<Key_T, Mapped_T>::SkipList *list;
        };
}

#endif