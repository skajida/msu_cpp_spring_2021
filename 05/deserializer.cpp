#include "deserializer.h"

EError TDeserializer::process(uint64_t &obj) {
    if (is >> obj) {
        if (int32_t next = is.get(); next == EOF || next == Separator) {
            return EError::NoError;
        }
    }
    return EError::CorruptedArchive;
}

EError TDeserializer::process(bool &obj) {
    if (is >> std::boolalpha >> obj >> std::noboolalpha) {
        if (int32_t next = is.get(); next == EOF || next == Separator) {
            return EError::NoError;
        }
    }
    return EError::CorruptedArchive;
}

TDeserializer::TDeserializer(std::istream &inputStream) : is(inputStream) {}
