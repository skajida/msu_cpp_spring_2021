#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include <array>

template <typename T>
std::string str(T &&ref) {
    std::ostringstream os;
    os << ref;
    return os.str();
}

template <typename... ArgsT>
std::string format(std::string_view formatString, ArgsT&&... args) {
    std::array<std::string, sizeof...(args)> formattedArgs = { str(std::forward<ArgsT>(args))... };
    std::ostringstream res;
    for (size_t openBrace, closeBrace; (openBrace = formatString.find('{')) != std::string::npos; formatString.remove_prefix(closeBrace + 1)) {
        closeBrace = formatString.find('}', openBrace + 1);
        if (closeBrace == std::string::npos) {
            throw std::runtime_error("couldn't find closing brace");
        }
        res << formatString.substr(0, openBrace);
        std::string inputSubString(formatString.substr(openBrace + 1, closeBrace - openBrace - 1));
        std::istringstream is(inputSubString);
        size_t arg;
        if (is >> std::noskipws >> arg && is.rdstate() == std::istream::eofbit && arg < formattedArgs.size()) {
            res << formattedArgs[arg];
        } else {
            throw std::runtime_error("key error: '" + inputSubString + "'");
        }
    }
    res << formatString;
    return res.str();
}
