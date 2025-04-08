
#include "logger.h"

using namespace exzvm::utility;

const char * Logger::s_level[LOG_COUNT] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

void Logger::open(const string &filename) {
    m_filename = filename;
    m_ofs.open(filename, std::ios::app);
    if (m_ofs.fail()) {
        throw std::logic_error("open log file failed: " + filename);
    }

    // 1.长度需要重置为0，要不然长度会一直叠加，后面就一直在翻滚
    // m_len = 0;

    // 2.文件指针设置到文件的结尾，这样设置比较合理
    m_ofs.seekp(0, std::ios::end);
    m_len = (int)m_ofs.tellp(); // tellp()：获取当前写入指针的位置，也就是文件当前的“长度”。把它赋值给 m_len，表示当前日志文件的字节数。
}

void Logger::set_level(Level level) {
    m_level = level;
}

void Logger::close() {
    m_ofs.close();
}

void Logger::set_max_size(int size) {
    m_max = size;
}

void Logger::set_console(bool console) {
    m_console = console;
}


void Logger::log(Level level, const char *file, int line, const char *format, ...) {
    // 低级别日志全部覆盖掉
    if (m_level > level) {
        return;
    }

    if (m_ofs.fail()) {
        return;
    }

    std::ostringstream oss;

    time_t ticks = time(nullptr);
    struct tm time_info = {0};
//    localtime_s(&time_info, &ticks); // 只在windows平台
    localtime_r(&ticks, &time_info);   // 只在Linux平台
    // 把时间戳转成年月日时分秒
    char timestamp[32] = {0};

    // 把时间戳转换为年月日时分秒
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &time_info);

//    std::cout << "时间戳=" << ticks << " 年月日=" << timestamp << std::endl;

    int len = 0;
    const char * fmt = "%s %s %s:%d ";
    // s_level[level]：通过int类型的数转换为字符串
    len = snprintf(nullptr, 0, fmt, timestamp, s_level[level], file, line);

    if (len > 0) {
        char * buffer = new char[len+1];
        snprintf(buffer, len+1, fmt, timestamp, s_level[level], file, line);
        buffer[len] = 0;
        oss << buffer;
        m_len += len;
        delete [] buffer;
    }

    // 获取 ... 参数
    va_list arg_ptr;
    va_start(arg_ptr, format);
    len = vsnprintf(nullptr, 0, format, arg_ptr);
    va_end(arg_ptr);
    if (len > 0) {
        char * content = new char[len+1];
        va_start(arg_ptr, format);
        vsnprintf(content, len+1, format, arg_ptr);
        va_end(arg_ptr);
        content[len] = 0;
        oss << content;
        m_len += len;
        delete [] content;
    }

    oss << "\n"; // 日志换行
    const string & str = oss.str();
//    std::cout << str << std::endl;
    // 写入到文件main.log当中
    m_ofs << str;
    m_ofs.flush();

    if (m_console) {
        std::cout << str;
    }

    if (m_max > 0 && m_len >= m_max) {
        // 日志需要翻滚
        rotate();
    }
}

void Logger::rotate() {
    close();
    sleep(1000); // 需要sleep 1秒，不然会重名
    time_t ticks = time(nullptr);
    struct tm time_info = {0};
    localtime_r(&ticks, &time_info);
    char timestamp[32] = {0};
    strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", &time_info);
    string backfilename = m_filename + timestamp;
    //
    if (rename(m_filename.c_str(), backfilename.c_str()) != 0) {
        throw std::logic_error("rename ini file failed");
    }

    open(m_filename);
}

void Logger::localtime(struct tm * time_info, const time_t * ticks) {
#ifdef WIN32
    localtime_s(time_info, ticks);
#else
    localtime_r(ticks, time_info);
#endif
}

void Logger::sleep(int milliseconds) {
#ifdef WIN32
    Sleep(milliseconds)
#else
    usleep(milliseconds*1000);
#endif
}