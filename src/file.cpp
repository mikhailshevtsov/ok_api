#include <ok/file.hpp>

#include <algorithm>

namespace ok
{

file::file() = default;

file::~file()
{
    close();
}

int file::read(char* data, std::size_t size)
{
    ssize_t read = 0, n = 0;
    while (read < size)
    {
        n = ::read(_fd, data + read, size - read);
        if (n <= 0)
            return n;
        read += n;
    }
    return read;
}

int file::write(const char* data, std::size_t size)
{
    ssize_t sent = 0, n = 0;
    while (sent < size)
    {
        n = ::write(_fd, data + sent, size - sent);
        if (n <= 0)
            return n;
        sent += n;
    }
    return sent;
}

int file::open(const path_t& path, mode _mode)
{
    close();
    _path = path;
    _fd = ::open(path.c_str(), _mode == mode::read ? O_RDONLY : O_WRONLY | O_CREAT | O_APPEND, 0644);
    return _fd;
}

int file::close()
{
    if (_fd < 0)
        return 0;
    int res = ::close(_fd);
    _fd = -1;
    return res;
}

const file::path_t& file::path() const
{
    return _path;
}

int file::fd() const
{
    return _fd;
}

std::size_t file::size() const
{
    return std::filesystem::file_size(_path);
}

bool file::is_open() const
{
    return _fd >= 0;
}

}