#include "format.h"

#include <iostream>
#include <cassert>

enum class EResult {
    OK,
    WA,
    RE
};

template <typename... ArgsT>
EResult test(std::string_view ans, std::string_view what, std::string_view formatString, ArgsT&&... args) {
    try {
        if (ans == format(formatString, std::forward<ArgsT&&>(args)...)) {
            return EResult::OK;
        }
        return EResult::WA;
    } catch (const format_error &e) {
        assert(e.what() == what);
        return EResult::RE;
    }
}

void TestFormat() {
    assert(test("https://github.com/skajiDa/msu_cpp_spring_2021", "", "https://github.com/{0}/msu_cpp_spring_2021", "skajiDa") == EResult::OK);
    assert(test("https://regex101.com/", "", "https://regex{0}.com/", 101) == EResult::OK);
    assert(test("one+one = 2", "", "{1}+{1} = {0}", 2, "one") == EResult::OK);
    assert(test("thiranger@mail.ru", "", "{1}@{2}", 938, "thiranger", "mail.ru") == EResult::OK);
    assert(test("", "", "") == EResult::OK);
    assert(test("no format", "", "no format") == EResult::OK);
    assert(test("0", "", "{0}", false) == EResult::OK);
    assert(test("750-938", "", "{0}{1}{2}{3}", 750, '-', 9, 38) == EResult::OK);
    assert(test("cheap 'n dale", "", "{10} 'n {11}", 0, true, false, 1, -1, 100, 6, 7, 8, 9, "cheap", "dale") == EResult::OK);

    assert(test("wrong answer", "", "{0} {1}", "wrong", "answeR") == EResult::WA);
    assert(test("wrong answer", "", "{0}{1}", "wrong", "answer") == EResult::WA);

    assert(test("lol_ooo@list.ru", "couldn't find closing brace", "{0@list.ru", "lol_ooo") == EResult::RE);
    assert(test("0}@list.ru", "found unexpected closing brace", "0}@list.ru") == EResult::RE);
    assert(test("0@list}.ru", "found unexpected closing brace", "0@list}.ru") == EResult::RE);
    assert(test("thiranger", "key error: ''", "thiran{}", "ger") == EResult::RE);
    assert(test("lol_ooo@list.ru", "key error: '2'", "{1}@{2}", "lol_ooo", "list.ru") == EResult::RE);
    assert(test("thiranger@mail.ru", "got unexpected keys", "{0}@{1}", "thiranger", "mail.ru", 938) == EResult::RE);
    assert(test("thiranger", "key error: 'string'", "thiran{string}", "ger") == EResult::RE);
    assert(test("thiranger", "key error: ' 0'", "thiran{ 0}", "ger") == EResult::RE);
    assert(test("thiranger", "key error: '0 '", "thiran{0 }", "ger") == EResult::RE);
    assert(test("thiranger", "key error: '0str'", "thiran{0str}", "ger") == EResult::RE);
    assert(test("thiranger", "key error: '348975342384721239872431092934902'", "thiran{348975342384721239872431092934902}", "ger") == EResult::RE);

    std::cerr << "TestFormat is OK" << std::endl;
}

int main() {
    TestFormat();
    return 0;
}
