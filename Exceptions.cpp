#include "Exceptions.hpp"


namespace oul
{

Exception::Exception() {
    this->line = -1;
    this->message = "";
    this->file = "";
}
Exception::Exception(const char * message) {
    this->line = -1;
    this->message = message;
    this->file = "";
}
Exception::Exception(int line, const char * file) {
    this->line = line;
    this->message = "";
    this->file = file;
}
Exception::Exception(const char * message, int line, const char * file) {
    this->message = message;
    this->line = line;
    this->file = file;
}

const char * Exception::what() const throw() {
    char * string = new char[255];
    if(line > -1) {
        sprintf(string, "%s \nException thrown at line %d in file %s", message, line, file);
        return string;
    } else {
        return message;
    }
}
void Exception::setLine(int line) {
    this->line = line;
}
void Exception::setFile(const char * file) {
    this->file = file;
}
void Exception::setMessage(const char * message) {
    this->message = message;
}

NoPlatformsInstalledException::NoPlatformsInstalledException() {
    setMessage("No platforms are installed on this system."
        "Check that OpenCL is installed and that ICD is registered properly");
}

NoValidPlatformsException::NoValidPlatformsException() {
    setMessage("No valid OpenCL platforms were found. Check the device criteria.");
}

} //namespace oul
