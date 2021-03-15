#include "token_parser.h"

#include <sstream>
#include <vector>
#include <cassert>

template <typename T>
void assertVector(const std::vector<T> &generated, const std::vector<T> &expected) {
    assert(generated.size() == expected.size());
    assert(std::equal(generated.begin(), generated.end(), expected.begin()));
}

void TestTokenParser() {
    {                                       // +14 theoretically accepted, but task doesnt mean that
        TTokenParser parser;                // -15 especially, so isdigit() is digestible
        std::istringstream input("              \
            test this_is_string_token_5 5_5     \
            5eriously 11 +14 -15 15 3218713     \
            lol_ooo@list.ru @thiranger @ska     \
            jiDa 5! = 120 draw circles owl      \
        ");
        std::vector<uint32_t> integer_found_tokens;
        std::vector<std::string> string_found_tokens;
        integer_found_tokens.reserve(4);
        string_found_tokens.reserve(15);
        parser.setStartCallback([] {
            std::cerr << "starting first test, the only one with setted startCallback" << std::endl;
        });
        parser.setEndCallback([] {
            std::cerr << "ending first test, the only one with setted endCallback" << std::endl;
        });
        parser.setDigitTokenCallback([&integer_found_tokens](uint32_t digitToken) {
            integer_found_tokens.push_back(digitToken);
        });
        parser.setStringTokenCallback([&string_found_tokens](std::string stringToken) {
            string_found_tokens.push_back(std::move(stringToken));
        });
        parser.parse(input);

        std::vector<uint32_t> integer_expected_tokens = { 11, 15, 3218713, 120 };
        std::vector<std::string> string_expected_tokens = {
            "test", "this_is_string_token_5", "5_5", "5eriously", "+14", "-15", "lol_ooo@list.ru",
            "@thiranger", "@ska", "jiDa", "5!", "=", "draw", "circles", "owl"
        };

        assertVector(integer_found_tokens, integer_expected_tokens);
        assertVector(string_found_tokens, string_expected_tokens);
    }
    {                                       // empty input
        TTokenParser parser;
        std::istringstream input("\t\n\n   \t  ");
        std::ostringstream output;
        parser.setDigitTokenCallback([&output](uint32_t digitToken) {
            output << digitToken;
        });
        parser.setStringTokenCallback([&output](std::string stringToken) {
            output << stringToken;
        });
        parser.parse(input);

        assert(output.str() == "");
    }
    std::cerr << "TestTokenParser is OK" << std::endl;
}

int main() {
    TestTokenParser();
    return 0;
}
