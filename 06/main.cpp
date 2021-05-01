#include "format.h"

#include <iostream>
#include <cassert>

enum class EResult {
    OK,
    WA,
    RE
};

template <typename... ArgsT>
EResult test(std::string_view ans, std::string_view formatString, ArgsT&&... args) {
    try {
        if (ans == format(formatString, std::forward<ArgsT&&>(args)...)) {
            return EResult::OK;
        }
        return EResult::WA;
    } catch (const std::runtime_error &e) {
        return EResult::RE;
    }
}

void TestFormat() {
    assert(test("https://github.com/skajiDa/msu_cpp_spring_2021", "https://github.com/{0}/msu_cpp_spring_2021", "skajiDa") == EResult::OK);
    assert(test("https://regex101.com/", "https://regex{0}.com/", 101) == EResult::OK);
    assert(test("one+one = 2", "{1}+{1} = {0}", 2, "one") == EResult::OK);
    assert(test("wrong answer", "{0} {1}", "wrong", "answeR") == EResult::WA);
    assert(test("wrong answer", "{0}{1}", "wrong", "answer") == EResult::WA);
    assert(test("lol_ooo@list.ru", "{0@list.ru", "lol_ooo") == EResult::RE);
    assert(test("thiranger", "thiran{}", "ger") == EResult::RE);
    assert(test("lol_ooo@list.ru", "{1}@{2}", "lol_ooo", "list.ru") == EResult::RE);

    std::cerr << "TestFormat is OK" << std::endl;
}

int main() {
    TestFormat();
    return 0;
}
