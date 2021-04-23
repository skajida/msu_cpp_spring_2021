#pragma once

#include "error.h"

#include <istream>

class TDeserializer {
private:
    static constexpr char Separator = ' ';
    std::istream &is;

    EError process(uint64_t&);
    EError process(bool&);

    template <typename T, typename... ArgsT>
    EError process(T &&obj, ArgsT&&... rest);
public:
    explicit TDeserializer(std::istream&);

    template <typename T>
    EError load(T &obj);

    template <typename... ArgsT>
    EError operator()(ArgsT&&... args);
};

template <typename T, typename... ArgsT>
EError TDeserializer::process(T &&obj, ArgsT&&... rest) {
    if (process(obj) == EError::CorruptedArchive) {
        return EError::CorruptedArchive;
    }
    return process(std::forward<ArgsT&&>(rest)...);
}

template <typename T>
EError TDeserializer::load(T &obj) {
    EError status = obj.serialize(*this);
    return is.rdstate() ? status : EError::CorruptedArchive;
}

template <typename... ArgsT>
EError TDeserializer::operator()(ArgsT&&... args) {
    return process(std::forward<ArgsT&&>(args)...);
}
