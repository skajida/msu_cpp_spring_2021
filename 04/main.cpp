#include "vector.h"
#include "bigint.h"

#include <random>
#include <sstream>
#include <array>
#include <vector>
#include <cassert>

void TestVector() {
    {
        TVector myVec;
        std::vector<uint32_t> stlVec;
        for (uint32_t i = 0; i < 1000; ++i) {
            myVec.push_back(i);
            stlVec.push_back(i);
        }
        assert(myVec.size() == stlVec.size());
        assert(myVec.capacity() == stlVec.capacity());
        assert(std::equal(myVec.begin(), myVec.end(), stlVec.begin()));
    }
    {
        TVector myVec;
        std::vector<uint32_t> stlVec;
        myVec.reserve(15);
        stlVec.reserve(15);
        assert(myVec.capacity() == stlVec.capacity());

        for (uint32_t i = 0; i < 16; ++i) {
            myVec.push_back(i);
            stlVec.push_back(i);
        }
        assert(myVec.back() == 15);

        myVec.pop_back();
        assert(myVec.capacity() > 15);
        assert(myVec.size() == 15);
        assert(myVec.capacity() == stlVec.capacity());
    }
    {
        TVector myVec;
        assert(myVec.empty());

        std::vector<uint32_t> stlVec;
        assert(myVec.capacity() == stlVec.capacity());
        assert(myVec.size() == stlVec.size());
        assert(myVec.begin() == stlVec.data());
    }
    {
        TVector myVec1 = { 1, 2, 3, 4, 5 };
        TVector myVec2 = { 6, 7, 8, 9, 0 };
        assert(!std::equal(myVec1.begin(), myVec1.end(), myVec2.begin()));

        TVector myVec3 = std::move(myVec1);
        assert(!myVec1.size() && !myVec1.capacity());
        std::vector<uint32_t> tmp = { 1, 2, 3, 4, 5 };
        assert(myVec3.size() == tmp.size());
        assert(std::equal(myVec3.rbegin(), myVec3.rend(), tmp.rbegin()));

        myVec1 = std::move(myVec2);
        assert(!myVec2.size() && !myVec2.capacity());
        tmp = { 6, 7, 8, 9, 0 };
        assert(myVec1.size() == tmp.size());
        assert(std::equal(myVec1.begin(), myVec1.end(), tmp.begin()));
    }
    std::cerr << "TestVector is OK" << std::endl;
}

std::string runCommand(const std::string &command) {
    std::ostringstream request;
    request << "echo '" << command << "' | ./script.py";
    FILE *ifs = popen(request.str().c_str(), "r");
    char buffer[512];
    if (fscanf(ifs, "%s", buffer) == 1) {
        fclose(ifs);
        return buffer;
    }
    fclose(ifs);
    return "";
}

void checkOperation(const std::vector<std::pair<std::string, std::string>> &operands, char op) {
    for (const auto &[lhs, rhs] : operands) {
        std::pair<TBigInt, TBigInt> numbers = std::make_pair(TBigInt(lhs), TBigInt(rhs));
        TBigInt product;
        switch (op) {
        case '+':
            product = numbers.first + numbers.second;
            break;
        case '-':
            product = numbers.first - numbers.second;
            break;
        case '*':
            product = numbers.first * numbers.second;
            break;
        default:
            break;
        }
        std::ostringstream os;
        os << product;
        assert(os.str() == runCommand(lhs + " " + op + " " + rhs));
    }
}

std::string generateRandomNumber(size_t length) {
    static std::mt19937 rand_gen(time(0));
    std::string res;
    res.reserve(length + 1);
    res.push_back(rand_gen() & 1 ? '+' : '-');
    for (size_t i = 0; i < length; ++i) {
        res.push_back('0' + (rand_gen() % 10));
    }
    return res;
}

std::string_view removePlusPrefix(std::string_view str) {
    if (str.size() && str.front() == '+') {
        str.remove_prefix(1);
    }
    return str;
}

void TestBigInt() {
    {                                       // comparing output with python script
        constexpr size_t operandsQuantity = 16;
        constexpr size_t randomNumberLength = 128;
        std::vector<std::pair<std::string, std::string>> terms(operandsQuantity);
        for (auto &[lhs, rhs] : terms) {
            lhs = generateRandomNumber(randomNumberLength);
            rhs = generateRandomNumber(randomNumberLength);
        }
        checkOperation(terms, '+');
        checkOperation(terms, '-');
        checkOperation(terms, '*');
    }
    {                                       // io operators
        std::array<std::istringstream, 4> inputs = {
            std::istringstream("3901381239408349345771209432747289178329484533713"),
            std::istringstream("+4093458238234294"),
            std::istringstream("-15"),
            std::istringstream("0")
        };
        TBigInt n;
        for (std::istringstream &input : inputs) {
            std::ostringstream output;
            input >> n;
            output << n;
            assert(removePlusPrefix(input.str()) == output.str());
        }
    }
    {                                       // unary minus
        std::vector<std::string> numbers = {
            "-34573947923842911239897459",
            "-1",
            "-0"
        };
        std::vector<std::string_view> numbersPos;
        numbersPos.reserve(numbers.size());
        for (const std::string &it : numbers) {
            numbersPos.push_back(std::string_view(it.begin() + 1, it.end()));
        }
        for (size_t i = 0; i < numbers.size(); ++i) {
            TBigInt neg(numbers[i]), pos(numbersPos[i]);
            assert(neg == -pos);
        }
    }
    {                                       // int32_t support
        TBigInt n = 17;
        assert(n == 17);
        assert(n + 100 == 117);
        assert(n - 100 == -83);
        assert(n * 100 == 1700);
    }
    {                                       // test from example
        std::ostringstream os;
        TBigInt a = 1;
        TBigInt b("123456789012345678901234567890");
        TBigInt c = a * b + 2;
        TBigInt d;
        d = std::move(c);
        a = d + b;
        os << a;
        assert(os.str() == "246913578024691357802469135782");
    }
    std::cerr << "TestBigInt is OK" << std::endl;
}

int main() {
    TestVector();
    TestBigInt();
    return 0;
}
