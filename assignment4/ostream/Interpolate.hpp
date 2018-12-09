//
// Created by Zachary Halpern on 5/4/18.
//

#ifndef ASSIGNMENT4_INTERPOLATE_HPP
#define ASSIGNMENT4_INTERPOLATE_HPP

#include <exception>
#include <iomanip>
#include <iostream>
#include <ios>
#include <tuple>
#include <vector>
#include <cstring>
#include <numeric>
#include <sstream>
#include <typeinfo>

#define DEBUG 0

namespace cs540
{
    struct WrongNumberOfArgs : public std::exception
    {
        const char* what() const throw()
        {
            return "Wrong num of args provided";
        }
    };

    std::string ffr(std::ostream& (*fp)(std::ostream&))
    {
        std::stringstream out;
        out << fp;
        return out.str();
    }

    template <typename T>
    bool isStringManipulator(T& argument)
    {
        std::stringstream tempBuffer;
        tempBuffer << argument;

        return (typeid(T) == typeid(std::ios_base& (*)(std::ios_base&)) || tempBuffer.str().empty());
    }

    // Non-templated Helper
    void Helper(std::stringstream& stream_vector, const std::string &argument_string)
    {
        std::size_t percent_location = -1;
        std::string string_builder("");

        if (DEBUG) std::cerr << "ARG:" << stream_vector.str() << " " << argument_string << std::endl;

        while (true)
        {
            percent_location = argument_string.find("%", percent_location + 1);
            if (DEBUG) std::cerr << "PL" << percent_location << std::endl;

            // No more %'s found
            if (percent_location == std::string::npos)
            {
                //if (DEBUG) std::cerr << "breaking!" << std::endl;
                break;
            }

            // If there's one at the beginning or it doesn't have slashes... something went a-miss.
            if (percent_location == 0 || argument_string.at(percent_location - 1) != '\\')
            {
               // if (DEBUG) std::cerr << percent_location << argument_string.at(percent_location - 1) << std::endl;
                if (DEBUG) std::cerr << "Calling from" << __LINE__ << std::endl;
                throw WrongNumberOfArgs();
            }

        }

        string_builder += argument_string;

        std::size_t found_escaped_percent = -1;
        while (true)
        {
            found_escaped_percent = string_builder.find("\\%", found_escaped_percent + 1);

            if (found_escaped_percent == std::string::npos)
            {
                break;
            }

            string_builder.replace(found_escaped_percent, 2, "%");
            found_escaped_percent--;
        }

        stream_vector << string_builder;
    }

    // Templated Helper
    template <typename T, typename ... Ts>
    void Helper(std::stringstream& stream_vector, const std::string &argument_string, T& argument, Ts&... other_arguments)
    {
        if (DEBUG) std::cerr << "Templated - " << argument << std::endl;
        if (typeid(T) == typeid(ffr))
        {
            if (DEBUG) std::cerr << "FFR" << std::endl;
            stream_vector << argument;

            if (DEBUG) std::cerr << typeid(argument).name() << stream_vector.str();
            Helper(stream_vector, argument_string, other_arguments...);
            return;
        }

        /*else if (typeid(T) == typeid(std::setw(0)))
        {
            stream_vector << argument;
            Helper(stream_vector, argument_string, other_arguments...);
            return;
        }
        else if (typeid(T) == typeid(std::setprecision(0)))
        {
            stream_vector << argument;
            Helper(stream_vector, argument_string, other_arguments...);
            return;
        }*/

        std::size_t percent_location = -1;
        std::string string_builder("");

        while (true)
        {
            percent_location = argument_string.find("%", percent_location + 1);
            if (DEBUG) std::cerr << "PLM: " << percent_location << std::endl;

            if (percent_location == std::string::npos)
            {
                if (isStringManipulator(argument))
                {
                    if (DEBUG) std::cerr << "isManip" << std::endl;
                    stream_vector << argument_string << argument;
                    Helper(stream_vector, argument_string, other_arguments...);
                    return;
                }
                else
                {
                    if (DEBUG) std::cerr << "Calling from" << __LINE__ << std::endl;
                    throw WrongNumberOfArgs();
                }
            }

            // No percent signs left OR we should leave it in place
            if (percent_location == 0 || argument_string.at(percent_location - 1) != '\\')
            {
                break;
            }
        }

        string_builder += argument_string.substr(0, percent_location);
        if (DEBUG) std::cerr << "BUILDER:" << string_builder << std::endl;

        std::size_t found_escaped_percent = -1;
        while (true)
        {
            found_escaped_percent = string_builder.find("\\%", found_escaped_percent + 1);
            if (DEBUG) std::cerr << found_escaped_percent << std::endl;

            // We have fixed all escaped percents to being un-escaped now
            if (found_escaped_percent == std::string::npos)
            {
                break;
            }

            string_builder.replace(found_escaped_percent, 2, "%");
            found_escaped_percent--;
        }

        if (DEBUG) std::cerr << "Appending:" << string_builder << std::endl;
        stream_vector << string_builder;

        if (isStringManipulator(argument))
        {
            if (DEBUG) std::cerr << "isSManp" << std::endl;
            stream_vector << argument;
            std::string remaining_string = argument_string.substr(percent_location);
            Helper(stream_vector, remaining_string, other_arguments...);
        }
        else
        {
            if (DEBUG) std::cerr << "NOT isSManp" << std::endl;
            stream_vector << argument;
            std::string remaining_string = argument_string.substr(percent_location + 1);
            Helper(stream_vector, remaining_string, other_arguments...);
        }
    }

    template <typename ... Ts>
    std::string Interpolate(const std::string &current, const Ts&... arguments)
    {
        std::stringstream final_result;
        final_result << "";

        Helper(final_result, current, arguments...);

        std::string remainder = final_result.str();

        return remainder;
    }
}

#endif //ASSIGNMENT4_INTERPOLATE_HPP