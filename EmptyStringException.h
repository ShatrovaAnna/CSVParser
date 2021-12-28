#ifndef EMPTYSTRINGEXCEPTION_H
#define EMPTYSTRINGEXCEPTION_H

using namespace std;

class EmptyStringException : public exception {
    string exception;
public:
    EmptyStringException(const string& error) : exception(error) {}
    const char* what() const noexcept override {
        return exception.c_str();
    }
};

#endif