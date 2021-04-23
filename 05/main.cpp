#include "serializer.h"
#include "deserializer.h"

#include <iostream>
#include <sstream>
#include <tuple>
#include <cassert>

struct TData1 {
    bool flag1;
    uint64_t number1;
    uint64_t number2;
    bool flag2;
    bool flag3;

    template <class Serializer>
    EError serialize(Serializer &serializer) {
        return serializer(flag1, number1, number2, flag2, flag3);
    }

    bool operator==(const TData1 &obj) const {
        return std::tie(flag1, number1, number2, flag2, flag3)
            == std::tie(obj.flag1, obj.number1, obj.number2, obj.flag2, obj.flag3);
    }

    bool operator!=(const TData1 &obj) const {
        return !(*this == obj);
    }
};

struct TData2 {
    bool flag1;
    uint64_t number1;
    bool flag2;

    template <class Serializer>
    EError serialize(Serializer &serializer) {
        return serializer(flag1, number1, flag2);
    }
};

struct TData3 {
    bool flag1;
    uint64_t number1;
    uint64_t number2;

    template <class Serializer>
    EError serialize(Serializer &serializer) {
        return serializer(flag1, number1, number2);
    }
};

void TestComplex() {
    {                                                   // correct structures
        TData1 objSource = { true, 1ull << 63, 1u << 31, false, false };
        TData1 objTarget = { false, 1ull << 63, (1u << 31) - 1, false, false };
        assert(objSource != objTarget);

        std::stringstream stream;
        TSerializer serializer(stream);
        EError status = serializer.save(objSource);
        assert(status == EError::NoError);
        assert(stream.str() == "true 9223372036854775808 2147483648 false false");

        TDeserializer deserializer(stream);
        status = deserializer.load(objTarget);
        assert(status == EError::NoError);
        assert(objSource == objTarget);
    }
    {                                                   // unintersected structures
        TData1 objSource = { true, 25, 52, true, false };
        TData2 objTarget = { false, 52, false };

        std::stringstream stream;
        TSerializer serializer(stream);
        EError status = serializer.save(objSource);
        assert(status == EError::NoError);
        assert(stream.str() == "true 25 52 true false");

        TDeserializer deserializer(stream);
        status = deserializer.load(objTarget);
        assert(status == EError::CorruptedArchive);
    }
    {                                                   // intersected structures
        TData1 objSource = { true, 25, 52, true, false };
        TData3 objTarget = { false, 25, 52 };

        std::stringstream stream;
        TSerializer serializer(stream);
        EError status = serializer.save(objSource);
        assert(status == EError::NoError);
        assert(stream.str() == "true 25 52 true false");

        TDeserializer deserializer(stream);
        status = deserializer.load(objTarget);
        assert(status == EError::CorruptedArchive);     // some data left
    }
    std::cerr << "TestComplex is OK" << std::endl;
}

int main() {
    TestComplex();
    return 0;
}
