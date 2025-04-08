#pragma once

namespace exzvm
{
    namespace utility
    {
        template <typename T>
        class Singleton
        {
        public:
            static T * instance()
            {
                static T instance; // C++11保证局部的静态数据是多线程安全的
                return &instance;
            }
        private:
            Singleton() = default;
            Singleton(const Singleton<T> &) = delete;
            Singleton<T> & operator = (const Singleton<T> &) = delete;
            ~Singleton() = default;
        };

        #define SINGLETON(classname)                                        \
            friend class Singleton<classname>;                              \
            private:                                                        \
                classname() = default;                                      \
                classname(const classname &) = delete;                      \
                classname & operator = (const classname &) = delete;        \
                ~classname() = default
    }
}