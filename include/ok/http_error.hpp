#ifndef OK_HTTPERROR_HPP
#define OK_HTTPERROR_HPP

#include <stdexcept>

namespace ok
{

class http_error : public std::runtime_error
{
public:
    http_error(const char* detail, int status = 500);

    int status() const;

private:
    int _status = 500;
};

}

#endif //OK_HTTPERROR_HPP