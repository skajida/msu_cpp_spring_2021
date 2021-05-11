#include "vector.h"

#include <iostream>
#include <vector>
#include <cassert>

struct TLog {
    uint32_t default_ctor;
    uint32_t int32_t_ctor;
    uint32_t copy_ctor;
    uint32_t move_ctor;
    uint32_t dtor;
    uint32_t copy_operator;
    uint32_t move_operator;

    bool operator==(const TLog &obj) const {
        return std::tie(default_ctor, int32_t_ctor, copy_ctor, move_ctor, dtor, copy_operator, move_operator)
            == std::tie(obj.default_ctor, obj.int32_t_ctor, obj.copy_ctor, obj.move_ctor, obj.dtor, obj.copy_operator, obj.move_operator);
    }
};

struct S {
    int32_t data;

    S() : data() { ++logs.default_ctor; }
    S(int32_t n) : data(n) { ++logs.int32_t_ctor; }
    S(const S &obj) : data(obj.data) { ++logs.copy_ctor; }
    S(S &&obj) : data(obj.data) { ++logs.move_ctor; }
    ~S() { ++logs.dtor; }
    S& operator=(const S &obj) { data = obj.data; ++logs.copy_operator; return *this; }
    S& operator=(S &&obj) { data = obj.data; ++logs.move_operator; return *this; }

    bool operator==(const S &obj) const { return data == obj.data; }

    static TLog dump() { TLog res = logs; logs = TLog(); return res; }
private:
    static TLog logs;
};

TLog S::logs{};

void TestVector() {
    {                                                   // same behavior as stl vector
        TLog logs[2];

        const TVector<S> myVector = { 0, -5, 4 };
        logs[0] = S::dump();

        const std::vector<S> stlVector = { 0, -5, 4 };
        logs[1] = S::dump();

        assert(myVector.capacity() == stlVector.capacity());
        assert(myVector.size() == stlVector.size());
        assert(std::equal(myVector.begin(), myVector.end(), stlVector.begin()));
        assert(logs[0] == logs[1]);
    }
    S::dump();                                          // cleared dtor counter
    {                                                   // mirrored vectors filled with no copy
        TLog logs[2];

        TVector<S> myVector;
        myVector.reserve(3);
        myVector.emplace_back(-1);
        myVector.emplace_back(0);
        myVector.emplace_back(1);
        logs[0] = S::dump();

        std::vector<S> stlVector;
        stlVector.reserve(3);
        stlVector.emplace_back(1);
        stlVector.emplace_back(0);
        stlVector.emplace_back(-1);
        logs[1] = S::dump();

        assert(myVector.capacity() == stlVector.capacity());
        assert(myVector.size() == stlVector.size());
        assert(std::equal(myVector.rbegin(), myVector.rend(), stlVector.begin()));
        assert(logs[0] == logs[1]);
    }
    S::dump();                                          // cleared dtor counter
    {                                                   // same dtor counter
        TLog logs[2];

        TVector<S> myVector = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
        myVector.clear();
        logs[0] = S::dump();

        std::vector<S> stlVector = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
        stlVector.clear();
        logs[1] = S::dump();

        assert(myVector.capacity() == stlVector.capacity());
        assert(myVector.size() == stlVector.size());
        assert(std::equal(myVector.rbegin(), myVector.rend(), stlVector.rbegin()));
        assert(logs[0] == logs[1]);
    }
    {                                                   // expected logic
        constexpr size_t size = 1'000'000;
        TVector<int32_t> myVector;
        myVector.reserve(size);
        assert(myVector.empty());
        myVector.resize(size);
        assert(myVector.size() == myVector.capacity());
        myVector.pop_back();
        assert(myVector.size() + 1 == myVector.capacity());
    }
    std::cerr << "TestVector is OK" << std::endl;
}

int main() {
    TestVector();
    return 0;
}
