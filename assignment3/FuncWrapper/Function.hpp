//
// Created by Zachary Halpern on 4/12/18.
//

#ifndef ASSIGNMENT3_FUNCTION_HPP
#define ASSIGNMENT3_FUNCTION_HPP

#include <cstddef>
#include <exception>
#include <memory>

namespace cs540
{
    struct BadFunctionCall : public std::exception
    {
        const char* what() const throw()
        {
            return "Bad Function Call Exception";
        }
    };

    template <typename>
    class Function
    {

    };

    template <typename ResultType, typename ... ArgumentTypes>
    class Function<ResultType(ArgumentTypes...)>
    {
        private:
            // https://shaharmike.com/cpp/naive-std-function/
            class Callable
            {
                public:
                    virtual ~Callable() = default;
                    virtual ResultType Invoke(ArgumentTypes...) = 0;
            };

            template <typename T>
            class CallableT : public Callable
            {
                public:
                    CallableT(const T& t) : t(t) {}

                    ~CallableT() override = default;

                    ResultType Invoke(ArgumentTypes... args) override
                    {
                        return t(args...);
                    }

                private:
                    T t;
            };

            std::shared_ptr<Callable> callable;

        public:
            Function() : callable(nullptr)
            {
            }

            template <typename FunctionType>
            Function(FunctionType function) : callable(std::make_shared<CallableT<FunctionType>>(function))
            {
            }


            Function(const Function &copy)
            {
                if (callable)
                {
                    callable.reset();
                }

                callable = copy.callable;
            }

            Function& operator=(const Function &copy)
            {
                 if (callable)
                 {
                     callable.reset();
                 }

                callable = copy.callable;
                return *this;
            }

            ~Function()
            {
                if (callable)
                {
                    callable.reset();
                }
            }

            ResultType operator()(ArgumentTypes... args)
            {
                if (callable)
                {
                    return callable->Invoke(args...);
                }
                else
                {
                    throw BadFunctionCall();
                }
            }

            explicit operator bool() const
            {
                return (callable != nullptr);
            }
    };

    template <typename ResultType, typename... ArgumentTypes>
    bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t)
    {
        return static_cast<bool>(f);
    }

    template <typename ResultType, typename... ArgumentTypes>
    bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f)
    {
        return static_cast<bool>(f);
    }

    template <typename ResultType, typename... ArgumentTypes>
    bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t)
    {
        return !f;
    }

    template <typename ResultType, typename... ArgumentTypes>
    bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f)
    {
        return !f;
    }

}

#endif //ASSIGNMENT3_FUNCTION_HPP
