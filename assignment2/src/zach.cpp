//
// Created by Zachary Halpern on 3/21/18.
//
#include "Map.hpp"
#include <chrono>
#include <random>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include <assert.h>
#include <map>
#include <initializer_list>
#include <set>
#include <cmath>

int main()
{
    cs540::Map<int, int> map;

    for (int i = 0; i < 10; i++)
    {
        map.insert(std::pair<int, int>(i, i));
    }

    for (cs540::Map<int, int>::Iterator iter = map.begin(); iter != map.end(); iter++)
    {
        iter->second *= 2;
        std::cout << iter->second << std::endl;
    }

    for (cs540::Map<int, int>::ConstIterator iter = map.begin(); iter != map.end(); iter++)
    {
        //iter->second *= 2;
        std::cout << iter->second << std::endl;
    }

    return 0;
}