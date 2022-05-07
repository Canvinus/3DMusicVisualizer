#include "exception.h"

Exception::Exception(std::string message) : message(std::move(message)) { }

const char* Exception::what() const noexcept {
    return message.data();
}