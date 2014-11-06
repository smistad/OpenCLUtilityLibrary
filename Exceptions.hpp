#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include "openclutilitylibrary_export.h"

#include <exception>
#include <cstdio>

namespace oul
{

class OPENCLUTILITYLIBRARY_EXPORT Exception : public std::exception {
    public:
        Exception();
        Exception(const char * message);
        Exception(int line, const char * file);
        Exception(const char * message, int line, const char * file);
        virtual const char * what() const throw();
        void setLine(int line);
        void setFile(const char * file);
        void setMessage(const char * message);

    private:
        int line;
        const char * file;
        const char * message;
};

class OPENCLUTILITYLIBRARY_EXPORT NoPlatformsInstalledException : public Exception {
    public:
        NoPlatformsInstalledException();
};

class OPENCLUTILITYLIBRARY_EXPORT NoValidPlatformsException : public Exception {
    public:
        NoValidPlatformsException();
};

} // end namespace oul
#endif
