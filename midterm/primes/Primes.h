//
// Created by Zachary Halpern on 3/28/18.
//

#ifndef TEST1_PRIMES_H
#define TEST1_PRIMES_H

#include <vector>
#include <iostream>

namespace cs540
{
    class Primes
    {
        public:
            class Iterator
            {
                public:
                    unsigned long index;
                    std::vector<unsigned long> *prime_values;

                    Iterator() = delete;
                    Iterator(int start, std::vector<unsigned long> *pvals) : index(start), prime_values(pvals) {}

                    Iterator& operator=(const Iterator &old)
                    {
                        index = old.index;
                        return *this;
                    }

                    Iterator &operator++()
                    {
                        index++;
                        return *this;
                    }

                    Iterator operator++(int)
                    {
                        Iterator iter(*this);
                        ++(*this);
                        return iter;
                    }

                    Iterator &operator--()
                    {
                        index--;
                        return *this;
                    }

                    Iterator operator--(int)
                    {
                        Iterator iter(*this);
                        --(*this);
                        return iter;
                    }

                    unsigned long operator*() const
                    {
                        return prime_values->at(index);
                    }

                    unsigned long* operator->() const
                    {
                        return &(prime_values->at(index));
                    }

                    friend bool operator!=(const Iterator &a, const Iterator &b)
                    {
                        return !(a == b);
                    }

                    friend bool operator==(const Iterator &a, const Iterator &b)
                    {
                        return (a.index == b.index);
                    }
            };

            Iterator begin()
            {
                return Iterator(0, &prime_values);
            }

            Iterator end()
            {
                return Iterator(prime_values.size(), &prime_values);
            }

            Primes() = delete;
            Primes(unsigned long total)
            {
                for (unsigned long i = 2; i <= total; i++)
                {
                    all_values.push_back(true);
                }

                for (unsigned long p = 2; p * p <= total; p++)
                {
                    // If prime[p] is not changed, then it is a prime
                    if (all_values[p] == true)
                    {
                        // Update all multiples of p
                        for (unsigned long i = p * 2; i <= total; i += p)
                        {
                            all_values[i] = false;
                        }
                    }
                }

                for (unsigned long p = 2; p <= total; p++)
                {
                    if (all_values[p])
                    {
                        prime_values.push_back(p);
                    }
                }
            }

        private:
            std::vector<bool> all_values;
            std::vector<unsigned long> prime_values;
    };
}

#endif //TEST1_PRIMES_H
