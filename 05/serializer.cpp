#include "serializer.h"

EError TSerializer::process(uint64_t obj) {
    os << obj;
    return EError::NoError;
}

EError TSerializer::process(bool obj) {
    os << std::boolalpha << obj << std::noboolalpha;
    return EError::NoError;
}

TSerializer::TSerializer(std::ostream &outputStream) : os(outputStream) {}
