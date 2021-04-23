#pragma once

#include "error.h"

#include <ostream>

class TSerializer {
private:
    static constexpr char Separator = ' ';
    std::ostream &os;

    EError process(uint64_t);
    EError process(bool);

    template <typename T, typename... ArgsT>
    EError process(T obj, ArgsT... rest);
public:
    explicit TSerializer(std::ostream&);

    template <typename T>
    EError save(T &obj);

    template <typename... ArgsT>
    EError operator()(ArgsT... args);
};

template <typename T, typename... ArgsT>
EError TSerializer::process(T obj, ArgsT... rest) {
    process(obj);
    os << Separator;
    return process(std::forward<ArgsT&>(rest)...);
}

template <typename T>
EError TSerializer::save(T &obj) {
    return obj.serialize(*this);
}

template <typename... ArgsT>
EError TSerializer::operator()(ArgsT... args) {
    return process(std::forward<ArgsT&>(args)...);
}
