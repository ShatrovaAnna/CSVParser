#ifndef LINEOUTOFRANGEEXCEPTION_H
#define LINEOUTOFRANGEEXCEPTION_H

using namespace std;

class LineOutOfRangeException : public exception {
    string exception;
public:
    LineOutOfRangeException(const string& error) : exception(error) {}
    const char* what() const noexcept override {
        return exception.c_str();
    }
};

#endif