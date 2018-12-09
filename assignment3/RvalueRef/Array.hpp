//
// Created by Zachary Halpern on 4/11/18.
//

#ifndef ASSIGNMENT3_ARRAY_HPP
#define ASSIGNMENT3_ARRAY_HPP

#include "MyInt.cpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <iostream>
#include <utility>
#include <vector>

namespace cs540
{
    class Array;
    std::ostream& operator<<(std::ostream& out, const Array &array);

    class Array
    {
        public:
            // COPY OPERATIONS
            Array(const std::initializer_list<MyInt> &copy)
            {
                for (const auto &val : copy)
                {
                    values.push_back(val);
                }
                array_size = copy.size();
            }

            Array(const Array &copy)
            {
                for (const auto &val : copy.values)
                {
                    values.push_back(val);
                }
                array_size = copy.size();
            }

            Array& operator=(const Array &copy)
            {
                for (const auto &val : copy.values)
                {
                    values.push_back(val);
                }
                array_size = copy.size();
                return *this;
            }

            // MOVE OPERATIONS
            Array(std::initializer_list<MyInt> &&move)
            {
                for (auto val : move)
                {
                    values.push_back(val);
                }
                array_size = move.size();
            }

            Array(Array &&move)
            {
                values = std::move(move.values);
                array_size = move.array_size;
            }

            Array& operator=(Array &&move)
            {
                values = std::move(move.values);
                array_size = move.array_size;
                return *this;
            }

            ~Array() = default;

            size_t size() const
            {
                return array_size;
            }

            const MyInt& getValueAt(size_t index) const
            {
                return values[index];
            }

            static void move_performance_test()
            {
                #define REVOLUTIONS 10000
                clock_t start_time, end_time;

                // performance test to verify that your move constructor and move assignment have
                // better performance compared to copy constructor and copy assignment.

                const std::initializer_list<MyInt> initializer = {1,3,5,7,9,11,2,4,6,8,10};

                const Array starting(initializer);

                {
                    start_time = clock();
                    for (int i = 0; i < REVOLUTIONS; i++)
                    {
                        Array tmp_move_ctor{std::move(starting)};
                        Array tmp_move_again = std::move(tmp_move_ctor);

                        Array tmp_move_assignment = std::move(starting);
                        Array tmp_move_init_list(initializer);
                    }
                    end_time = clock();

                    std::cout << "Move Time: " << end_time - start_time << std::endl;
                }

                {
                    start_time = clock();
                    for (int i = 0; i < REVOLUTIONS; i++)
                    {
                        Array tmp_copy_ctor{starting};
                        Array tmp_copy_again = tmp_copy_ctor;
                        Array tmp_copy_assignment = starting;
                        Array tmp_copy_init_list(initializer);
                    }
                    end_time = clock();

                    std::cout << "Copy Time: " << end_time - start_time << std::endl;
                }
            }

        private:
            size_t array_size;
            std::vector<MyInt> values;
    };

    std::ostream& operator<<(std::ostream& out, const Array &array)
    {
        const size_t addComma = array.size() - 1;
        for (size_t i = 0; i < array.size(); i++)
        {
            out << array.getValueAt(i);
            if (i < addComma)
            {
                out << ", ";
            }
        }

        return out;
    }
}

#endif //ASSIGNMENT3_ARRAY_HPP
