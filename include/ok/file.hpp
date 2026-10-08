#ifndef OK_FILE_HPP
#define OK_FILE_HPP

#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <string_view>
#include <vector>

namespace ok
{

class file
{
public:
    enum class mode { read, write };

    using path_t = std::filesystem::path;

    file();
    ~file();

    int read(char* data, std::size_t size);
    int write(const char* data, std::size_t size);

    int open(const path_t& path, mode _mode);
    int close();

    const path_t& path() const;
    int fd() const;
    std::size_t size() const;

    bool is_open() const;

public:
    static constexpr std::size_t threshold() { return 64 * 1024; };

private:
    path_t _path;
    int _fd = -1;
};

}

#endif //OK_FILE_HPP