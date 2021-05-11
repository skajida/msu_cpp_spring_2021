#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include <array>
#include <bitset>

class format_error : public std::logic_error {
public:
    explicit format_error(std::string message) : logic_error(message) {}
};

template <typename T>
std::string str(T &&ref) {
    std::ostringstream os;
    os << ref;
    return os.str();
}

template <typename... ArgsT>
std::string format(std::string_view formatString, ArgsT&&... args) {
    std::array<std::string, sizeof...(args)> formattedArgs = { str(std::forward<ArgsT>(args))... };
    std::bitset<sizeof...(args)> usedArgs;
    std::ostringstream res;
    for (size_t openBrace, closeBrace; (openBrace = formatString.find('{')) != std::string::npos; formatString.remove_prefix(closeBrace + 1)) {
        closeBrace = formatString.find('}', openBrace + 1);
        if (closeBrace == std::string::npos) {
            throw format_error("couldn't find closing brace");
        }
        res << formatString.substr(0, openBrace);
        std::string inputSubString(formatString.substr(openBrace + 1, closeBrace - openBrace - 1));
        std::istringstream is(inputSubString);
        size_t arg;
        if (is >> std::noskipws >> arg && is.rdstate() == std::istream::eofbit && arg < formattedArgs.size()) {
            res << formattedArgs[arg];
            usedArgs[arg] = true;
        } else {
            throw format_error("key error: '" + inputSubString + "'");
        }
    }
    if (formatString.find('}') != std::string_view::npos) {
        throw format_error("found unexpected closing brace");
    }
    if (sizeof...(args) && !usedArgs[sizeof...(args) - 1]) {
        throw format_error("got unexpected keys");
    }
    res << formatString;
    return res.str();
}
