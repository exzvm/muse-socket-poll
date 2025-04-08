#pragma once
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <cstdarg>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <utility/singleton.h>

using std::string;

namespace exzvm
{
    namespace utility
    {
        // 定义宏，使得debug的时候不需要写太多参数
        #define log_debug(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

        #define log_info(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

        #define log_warn(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

        #define log_error(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

        #define log_fatal(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

        class Logger
        {
            SINGLETON(Logger);
        public:
            enum Level
            {
                LOG_DEBUG = 0,
                LOG_INFO,
                LOG_WARN,
                LOG_ERROR,
                LOG_FATAL,
                LOG_COUNT
            };

            void open(const string &filename);
            void close();
            void log(Level level, const char *file, int line, const char *format, ...);
            void set_level(Level level);
            void set_max_size(int size);
            void set_console(bool console);

        private:

            void rotate();
            void localtime(struct tm * time_info, const time_t * ticks);
            void sleep(int milliseconds);

        private:
            string m_filename; // 文件名
            std::ofstream m_ofs; // 写入文件
            int m_max = 0; // log文件最大值
            int m_len = 0;
            int m_level = LOG_DEBUG;
            bool m_console = true;
            static const char * s_level[LOG_COUNT];

        };
    }
}
