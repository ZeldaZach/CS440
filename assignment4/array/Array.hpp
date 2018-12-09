//
// Created by Zachary Halpern on 4/29/18.
//

#ifndef ASSIGNMENT4_ARRAY_HPP
#define ASSIGNMENT4_ARRAY_HPP

#include <exception>

namespace cs540
{
    struct OutOfRange : public std::exception
    {
        const char *what() const throw()
        {
            return "Out of Array Range";
        }
    };

    template <typename T, std::size_t D, std::size_t... Dims>
    class Array
    {
        static_assert(D > 0);

        public:
            std::size_t size;
            Array<T, Dims...> *arrays;
            static T ValueType;

            Array() : size(D), arrays(new Array<T, Dims...>[size])
            {
            }

            Array(const Array &old) : size(old.size), arrays(new Array<T, Dims...>[size])
            {
                for (std::size_t i = 0; i < size; i++)
                {
                    arrays[i] = old.arrays[i];
                }
            }

            template <typename U>
            Array(const Array<U, D, Dims...> &old) : size(old.size), arrays(new Array<T, Dims...>[size])
            {
                for (std::size_t i = 0; i < size; i++)
                {
                    arrays[i] = old.arrays[i];
                }
            }

            ~Array()
            {
                delete[] arrays;
            }

            Array& operator=(const Array &copy)
            {
                if (this != &copy)
                {
                    if (size != copy.size)
                    {
                        throw OutOfRange();
                    }

                    for (std::size_t i = 0; i < size; i++)
                    {
                        arrays[i] = copy.arrays[i];
                    }
                }

                return *this;
            }

            template <typename U>
            Array& operator=(const Array<U, D, Dims...> &copy)
            {
                if (size != copy.size)
                {
                    throw OutOfRange();
                }

                for (std::size_t i = 0; i < size; i++)
                {
                    arrays[i] = copy.arrays[i];
                }

                return *this;
            }

            Array<T, Dims...>& operator[](const std::size_t i)
            {
                if (i >= size)
                {
                    throw OutOfRange();
                }

                return arrays[i];
            }

            const Array<T, Dims...>& operator[](const std::size_t i) const
            {
                if (i >= size)
                {
                    throw OutOfRange();
                }

                return arrays[i];
            }

            /*
             * This will recurse on itself if we put a lower order in
             * Array<T, 1,2,3,4> will then call Array<T, 1,2,3> and so on
             * https://stackoverflow.com/questions/6588487/is-there-a-way-to-iterate-over-an-n-dimensional-array-where-n-is-variable-with
             */
            class FirstDimensionMajorIterator
            {
                public:
                    std::size_t size, curr_index;
                    bool isEndOfIndex;
                    Array<T, D, Dims...>* arrays;
                    typename Array<T, Dims...>::FirstDimensionMajorIterator lowerOrderIterator;

                public:
                    FirstDimensionMajorIterator() :
                            size(D), curr_index(0),
                            isEndOfIndex(false), arrays(nullptr)
                    {
                    }

                    FirstDimensionMajorIterator(const FirstDimensionMajorIterator &old) = default;

                    FirstDimensionMajorIterator& operator=(const FirstDimensionMajorIterator &copy) = default;

                    bool operator==(const FirstDimensionMajorIterator &other)
                    {
                        // If any of the values don't match, we're done
                        // otherwise, see if we're at the end of the
                        // chain. If not, go to the next layer
                        if (
                                (size != other.size) ||
                                (curr_index != other.curr_index) ||
                                (isEndOfIndex != other.isEndOfIndex) ||
                                (arrays != other.arrays)
                            )
                        {
                            return false;
                        }
                        else if (isEndOfIndex)
                        {
                            return true;
                        }
                        else
                        {
                            // Compares next layer of arrays
                            return lowerOrderIterator == other.lowerOrderIterator;
                        }
                    }

                    bool operator!=(const FirstDimensionMajorIterator &other)
                    {
                        return !(operator==(other));
                    }

                    FirstDimensionMajorIterator& operator++()
                    {
                        lowerOrderIterator++;
                        if (lowerOrderIterator.isEndOfIndex && lowerOrderIterator.curr_index == 0)
                        {
                            if (++curr_index == size)
                            {
                                curr_index = 0;
                                isEndOfIndex = true;
                            }
                            else
                            {
                                isEndOfIndex = false;
                            }

                            // Go to the next layer down
                            lowerDimension();
                        }

                        return *this;
                    }

                    FirstDimensionMajorIterator operator++(int)
                    {
                        auto temp = *this;
                        ++(*this); // Call operator++()
                        return temp;
                    }

                    T& operator*() const
                    {
                        return *lowerOrderIterator;
                    }

                    void lowerDimension()
                    {
                        lowerOrderIterator.arrays = &(arrays->arrays[curr_index]);
                        lowerOrderIterator.isEndOfIndex = false;
                        lowerOrderIterator.lowerDimension();
                    }
            };

            class LastDimensionMajorIterator
            {
                public:
                    std::size_t size, curr_index;
                    bool isEndOfIndex;
                    Array<T, D, Dims...>* arrays;
                    typename Array<T, Dims...>::LastDimensionMajorIterator lowerOrderIterator;

                public:
                    LastDimensionMajorIterator() :
                            size(D), curr_index(0),
                            isEndOfIndex(false), arrays(nullptr)
                    {
                    }

                    LastDimensionMajorIterator(const LastDimensionMajorIterator &old) = default;

                    LastDimensionMajorIterator& operator=(const LastDimensionMajorIterator &copy) = default;

                    bool operator==(const LastDimensionMajorIterator &other)
                    {
                        if (
                                (size != other.size) ||
                                (curr_index != other.curr_index) ||
                                (isEndOfIndex != other.isEndOfIndex) ||
                                (arrays != other.arrays)
                            )
                        {
                            return false;
                        }
                        else if (isEndOfIndex)
                        {
                            return true;
                        }
                        else
                        {
                            // Compare the next layer down
                            return lowerOrderIterator == other.lowerOrderIterator;
                        }
                    }

                    bool operator!=(const LastDimensionMajorIterator &other)
                    {
                        return !(operator==(other));
                    }

                    LastDimensionMajorIterator& operator++()
                    {
                        curr_index = (curr_index + 1) % size;
                        if (curr_index == 0)
                        {
                            isEndOfIndex = true;
                            lowerOrderIterator++;
                        }
                        lowerDimension();

                        return *this;
                    }

                    LastDimensionMajorIterator operator++(int)
                    {
                        auto temp = *this;
                        ++(*this);
                        return temp;
                    }

                    T& operator*() const
                    {
                        return *lowerOrderIterator;
                    }

                    void lowerDimension()
                    {
                        lowerOrderIterator.arrays = &(arrays->arrays[curr_index]);
                        lowerOrderIterator.lowerDimension();
                        isEndOfIndex = lowerOrderIterator.isEndOfIndex;
                    }
            };

            FirstDimensionMajorIterator fmbegin()
            {
                FirstDimensionMajorIterator it;
                it.arrays = this;
                it.isEndOfIndex = false;
                it.lowerOrderIterator = arrays[0].fmbegin();
                return it;
            }

            FirstDimensionMajorIterator fmend()
            {
                FirstDimensionMajorIterator it;
                it.arrays = this;
                it.isEndOfIndex = true;
                it.lowerOrderIterator = arrays[0].fmend();
                return it;
            }

            LastDimensionMajorIterator lmbegin()
            {
                LastDimensionMajorIterator it;
                it.arrays = this;
                it.isEndOfIndex = false;
                it.lowerOrderIterator = arrays[0].lmbegin();
                return it;
            }

            LastDimensionMajorIterator lmend()
            {
                LastDimensionMajorIterator it;
                it.arrays = this;
                it.isEndOfIndex = true;
                it.lowerOrderIterator = arrays[0].lmend();
                return it;
            }
    };

    // Base
    template <typename T, std::size_t D>
    class Array<T, D>
    {
        static_assert(D > 0);

        public:
            std::size_t size;
            T *arrays;
            static T ValueType;
            class FirstDimensionMajorIterator;
            class LastDimensionMajorIterator;

            Array() : size(D), arrays(new T[size])
            {
            }

            Array(const Array &old) : size(old.size), arrays(new T[size])
            {
                for (std::size_t i = 0; i < size; i++)
                {
                    arrays[i] = old.arrays[i];
                }
            }

            template <typename U>
            Array(const Array<U, D> &old) : size(old.size), arrays(new T[size])
            {
                for (std::size_t i = 0; i < size; i++)
                {
                    arrays[i] = old.arrays[i];
                }
            }

            ~Array()
            {
                delete[] arrays;
            }

            Array& operator=(const Array &copy)
            {
                if (this != &copy)
                {
                    size = copy.size;

                    for (std::size_t i = 0; i < size; i++)
                    {
                        arrays[i] = copy.arrays[i];
                    }
                }

                return *this;
            }

            template <typename U>
            Array& operator=(const Array<U, D> &copy)
            {
                size = copy.size;

                for (std::size_t i = 0; i < size; i++)
                {
                    arrays[i] = copy.arrays[i];
                }

                return *this;
            }

            T& operator[](const std::size_t i)
            {
                if (i >= size)
                {
                    throw OutOfRange();
                }

                return arrays[i];
            }

            const T& operator[](const std::size_t i) const
            {
                if (i >= size)
                {
                    throw OutOfRange();
                }

                return arrays[i];
            }

            class FirstDimensionMajorIterator
            {
                public:
                    std::size_t size, curr_index;
                    bool isEndOfIndex;
                    Array<T, D>* arrays;
                    T *data;

                public:
                    FirstDimensionMajorIterator() :
                            size(D), curr_index(0),
                            isEndOfIndex(false), arrays(nullptr),
                            data(nullptr)
                    {

                    }

                    FirstDimensionMajorIterator(const FirstDimensionMajorIterator &old) = default;

                    FirstDimensionMajorIterator& operator=(const FirstDimensionMajorIterator &copy) = default;

                    bool operator==(const FirstDimensionMajorIterator &other)
                    {
                        if (
                                (size != other.size) ||
                                (curr_index != other.curr_index) ||
                                (isEndOfIndex != other.isEndOfIndex) ||
                                (arrays != other.arrays) ||
                                (data != other.data)
                            )
                        {
                            return false;
                        }
                        else if (isEndOfIndex)
                        {
                            return true;
                        }
                        else
                        {
                            return data == other.data;
                        }
                    }

                    bool operator!=(const FirstDimensionMajorIterator &other)
                    {
                        return !(operator==(other));
                    }

                    FirstDimensionMajorIterator& operator++()
                    {
                        if (++curr_index == size)
                        {
                            curr_index = 0;
                            isEndOfIndex = true;
                        }
                        else
                        {
                            isEndOfIndex = false;
                        }

                        lowerDimension();

                        return *this;
                    }

                    FirstDimensionMajorIterator operator++(int)
                    {
                        auto temp = *this;
                        ++(*this);
                        return temp;
                    }

                    T& operator*() const
                    {
                        return *data;
                    }

                    void lowerDimension()
                    {
                        data = &(arrays->arrays[curr_index]);
                    }
            };

            class LastDimensionMajorIterator
            {
                public:
                    std::size_t size, curr_index;
                    bool isEndOfIndex;
                    Array<T, D>* arrays;
                    T *data;

                public:
                    LastDimensionMajorIterator() :
                            size(D), curr_index(0),
                            isEndOfIndex(false), arrays(nullptr),
                            data(nullptr)
                    {
                    }

                    LastDimensionMajorIterator(const LastDimensionMajorIterator &old) = default;

                    LastDimensionMajorIterator& operator=(const LastDimensionMajorIterator &copy) = default;

                    bool operator==(const LastDimensionMajorIterator &other)
                    {
                        if (
                                (size != other.size) ||
                                (curr_index != other.curr_index) ||
                                (isEndOfIndex != other.isEndOfIndex) ||
                                (data != other.data)
                            )
                        {
                            return false;
                        }
                        else if (isEndOfIndex)
                        {
                            return true;
                        }
                        else
                        {
                            return data == other.data;
                        }
                    }

                    bool operator!=(const LastDimensionMajorIterator &other)
                    {
                        return !(operator==(other));
                    }

                    LastDimensionMajorIterator& operator++()
                    {
                        if (++curr_index == size)
                        {
                            curr_index = curr_index % size;
                            isEndOfIndex = true;
                        }
                        else
                        {
                            isEndOfIndex = false;
                        }

                        lowerDimension();
                        return *this;
                    }

                    LastDimensionMajorIterator operator++(int)
                    {
                        auto temp = *this;
                        ++(*this);
                        return temp;
                    }

                    T& operator*() const
                    {
                        return *data;
                    }

                    void lowerDimension()
                    {
                        data = &(arrays->arrays[curr_index]);
                    }
            };

            FirstDimensionMajorIterator fmbegin()
            {
                FirstDimensionMajorIterator it;
                it.arrays = this;
                it.data = &(arrays[0]);
                it.isEndOfIndex = false;
                return it;
            }

            FirstDimensionMajorIterator fmend()
            {
                FirstDimensionMajorIterator it;
                it.arrays = this;
                it.data = &(arrays[0]);
                it.isEndOfIndex = true;
                return it;
            }

            LastDimensionMajorIterator lmbegin()
            {
                LastDimensionMajorIterator it;
                it.arrays = this;
                it.data = &(arrays[0]);
                it.isEndOfIndex = false;
                return it;
            }

            LastDimensionMajorIterator lmend()
            {
                LastDimensionMajorIterator it;
                it.arrays = this;
                it.data = &(arrays[0]);
                it.isEndOfIndex = true;
                return it;
            }
    };
}

#endif //ASSIGNMENT4_ARRAY_HPP
