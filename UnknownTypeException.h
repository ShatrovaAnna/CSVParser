#ifndef UNKNOWNTYPEEXCEPTION_H
#define UNKNOWNTYPEEXCEPTION_H

using namespace std;

class UnknownTypeException : public exception {
    string exception;
public:
    UnknownTypeException(const string& error) : exception(error) {}
    const char* what() const noexcept override {
        return exception.c_str();
    }
};

#endif