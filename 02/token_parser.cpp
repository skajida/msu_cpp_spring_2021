#include "token_parser.h"

#include <algorithm>
#include <cctype>

TTokenParser::TTokenParser()
    : startCallback([] {})
    , endCallback([] {})
    , digitTokenCallback([](uint64_t) {})
    , stringTokenCallback([](std::string) {})
{
    //
}

void TTokenParser::setStartCallback(std::function<void()> func) {
    func = func == nullptr ? [] {} : func;
    startCallback = func;
}

void TTokenParser::setEndCallback(std::function<void()> func) {
    func = func == nullptr ? [] {} : func;
    endCallback = func;
}

void TTokenParser::setDigitTokenCallback(std::function<void(uint64_t)> func) {
    func = func == nullptr ? [](uint64_t) {} : func;
    digitTokenCallback = func;
}

void TTokenParser::setStringTokenCallback(std::function<void(std::string)> func) {
    func = func == nullptr ? [](std::string) {} : func;
    stringTokenCallback = func;
}

void TTokenParser::parse(std::istream &is) const {
    startCallback();
    std::string str;
    while (is >> str) {
        if (std::all_of(str.begin(), str.end(), isdigit)) {
            try {
                digitTokenCallback(std::stoull(str));
            } catch (const std::out_of_range &e) {
                stringTokenCallback(std::move(str));
            }
        } else {
            stringTokenCallback(std::move(str));
        }
    }
    endCallback();
}
