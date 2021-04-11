#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <cstdint>

class TTokenParser {
private:
    std::function<void()> startCallback;
    std::function<void()> endCallback;
    std::function<void(uint64_t)> digitTokenCallback;
    std::function<void(std::string)> stringTokenCallback;
public:
    TTokenParser();

    void setStartCallback(std::function<void()>);
    void setEndCallback(std::function<void()>);
    void setDigitTokenCallback(std::function<void(uint64_t)>);
    void setStringTokenCallback(std::function<void(std::string)>);

    void parse(std::istream&) const;
};
