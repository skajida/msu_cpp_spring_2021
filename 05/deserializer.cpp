#include "deserializer.h"

EError TDeserializer::process(uint64_t &obj) {
    if ((is >> obj).ignore(1)) {
        return EError::NoError;
    }
    return EError::CorruptedArchive;
}

EError TDeserializer::process(bool &obj) {
    if ((is >> std::boolalpha >> obj >> std::noboolalpha).ignore(1)) {
        return EError::NoError;
    }
    return EError::CorruptedArchive;
}

TDeserializer::TDeserializer(std::istream &inputStream) : is(inputStream) {}
