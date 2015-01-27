#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <stdexcept>

namespace typing
{
    class MemoryAllocationError : public std::runtime_error
    {
    public:
        MemoryAllocationError (const std::string& msg) : std::runtime_error(msg) {}
    };

    class FileNotFoundException : public std::runtime_error
    {
    public:
        FileNotFoundException (const std::string& msg) : std::runtime_error(msg) {}
    };

    class FileCorruptException : public std::runtime_error
    {
    public:
        FileCorruptException (const std::string& msg) : std::runtime_error(msg) {}
    };

    class FileWriteException : public std::runtime_error
    {
    public:
        FileWriteException (const std::string& msg) : std::runtime_error(msg) {}
    };

    class MediaNotLoadedException : public std::runtime_error
    {
    public:
        MediaNotLoadedException (const std::string& msg) : std::runtime_error(msg) {}
    };
}

#endif // _EXCEPTIONS_H_
