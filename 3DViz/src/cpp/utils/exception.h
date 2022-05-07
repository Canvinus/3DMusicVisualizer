#pragma once

#include <string>

/**
 * The exception with the custom message
 */
class Exception : public std::exception {

private:

    std::string message;

public:

    /**
     * Constructor
     * @param message the custom message
     */
    explicit Exception(std::string message);

    /**
     * Returns the custom message
     * @return the custom message
     */
    const char* what() const noexcept override;

};
