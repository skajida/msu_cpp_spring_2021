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

struct TData4 {
    bool flag1;

    template <class Serializer>
    EError serialize(Serializer &serializer) {
        return serializer(flag1);
    }

    bool operator==(const TData4 &obj) const {
        return flag1 == obj.flag1;
    }
};

struct TData5 {
    uint64_t number1;

    template <class Serializer>
    EError serialize(Serializer &serializer) {
        return serializer(number1);
    }

    bool operator==(const TData5 &obj) const {
        return number1 == obj.number1;
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
    {                                                   // intersected structures / data oversupply
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
    {                                                   // lack of data
        TData1 objTarget;
        std::istringstream input("true 25 52 true");
        TDeserializer deserializer(input);
        EError status = deserializer.load(objTarget);
        assert(status == EError::CorruptedArchive);
    }
    {                                                   // single bool structure
        TData4 objSource = { false };
        TData4 objTarget = { true };

        std::stringstream stream;
        TSerializer serializer(stream);
        EError status = serializer.save(objSource);
        assert(status == EError::NoError);
        assert(stream.str() == "false");

        TDeserializer deserializer(stream);
        status = deserializer.load(objTarget);
        assert(status == EError::NoError);
        assert(objSource == objTarget);
    }
    {                                                   // single uint64_t structure
        TData5 objSource = { 39485290349186348 };
        TData5 objTarget;

        std::stringstream stream;
        TSerializer serializer(stream);
        EError status = serializer.save(objSource);
        assert(status == EError::NoError);
        assert(stream.str() == "39485290349186348");

        TDeserializer deserializer(stream);
        status = deserializer.load(objTarget);
        assert(status == EError::NoError);
        assert(objSource == objTarget);
    }
    {                                                   // noboolalpha input for boolean tokens
        TData4 objTarget;

        std::istringstream input_boolalpha("true");
        TDeserializer deserializer_boolalpha(input_boolalpha);
        EError status = deserializer_boolalpha.load(objTarget);
        assert(status == EError::NoError);

        std::istringstream input_noboolalpha("1");
        TDeserializer deserializer_noboolalpha(input_noboolalpha);
        status = deserializer_noboolalpha.load(objTarget);
        assert(status == EError::CorruptedArchive);
    }
    {                                                   // big integer input
        TData5 objTarget;
        std::istringstream input("91287231123093324762381212876543489234");
        TDeserializer deserializer(input);
        EError status = deserializer.load(objTarget);
        assert(status == EError::CorruptedArchive);
    }
    {
        TData3 objTarget;
        std::istringstream input("false938 750");
        TDeserializer deserializer(input);
        EError status = deserializer.load(objTarget);
        assert(status == EError::CorruptedArchive);
    }
    {
        TData5 objTarget;
        std::istringstream input("0");
        TDeserializer deserializer(input);
        EError status = deserializer.load(objTarget);
        assert(status == EError::NoError);
    }
    std::cerr << "TestComplex is OK" << std::endl;
}

int main() {
    TestComplex();
    return 0;
}
