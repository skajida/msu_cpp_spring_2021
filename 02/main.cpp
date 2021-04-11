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
        std::istringstream input("          \
            test this_is_string_token_5 5_5 \
            5eriously 11 +14 -15 15 3218713 \
            lol_ooo@list.ru @thiranger @ska \
            jiDa 5! = 120 draw circles owl  \
            0 a  \t 1                       \
        ");
        std::ostringstream startCallbackOS;
        std::ostringstream endCallbackOS;
        std::vector<uint64_t> integer_found_tokens;
        std::vector<std::string> string_found_tokens;
        integer_found_tokens.reserve(6);
        string_found_tokens.reserve(16);
        std::string startCallbackStr = "starting first test, the only one with setted startCallback\n";
        std::string endCallbackStr = "ending first test, the only one with setted endCallback\n";
        parser.setStartCallback([&startCallbackOS, &startCallbackStr] {
            startCallbackOS << startCallbackStr;
        });
        parser.setEndCallback([&endCallbackOS, &endCallbackStr] {
            endCallbackOS << endCallbackStr;
        });
        parser.setDigitTokenCallback([&integer_found_tokens](uint32_t digitToken) {
            integer_found_tokens.push_back(digitToken);
        });
        parser.setStringTokenCallback([&string_found_tokens](std::string stringToken) {
            string_found_tokens.push_back(std::move(stringToken));
        });
        parser.parse(input);

        std::vector<uint64_t> integer_expected_tokens = { 11, 15, 3218713, 120, 0, 1 };
        std::vector<std::string> string_expected_tokens = {
            "test", "this_is_string_token_5", "5_5", "5eriously", "+14", "-15", "lol_ooo@list.ru",
            "@thiranger", "@ska", "jiDa", "5!", "=", "draw", "circles", "owl", "a"
        };

        assert(startCallbackOS.str() == startCallbackStr);
        assertVector(integer_found_tokens, integer_expected_tokens);
        assertVector(string_found_tokens, string_expected_tokens);
        assert(endCallbackOS.str() == endCallbackStr);
    }
    {                                       // empty input
        TTokenParser parser;
        std::istringstream input("\t\n\n   \t  ");
        std::ostringstream output;
        parser.setDigitTokenCallback([&output](uint64_t digitToken) {
            output << digitToken;
        });
        parser.setStringTokenCallback([&output](std::string stringToken) {
            output << stringToken;
        });
        parser.parse(input);

        assert(output.str() == "");
    }
    {                                       // 2 ^ 64
        TTokenParser parser;
        std::istringstream input("18446744073709551615 18446744073709551616");
        std::vector<uint64_t> integer_found_tokens;
        std::vector<std::string> string_found_tokens;
        parser.setDigitTokenCallback([&integer_found_tokens](uint64_t digitToken) {
            integer_found_tokens.push_back(digitToken);
        });
        parser.setStringTokenCallback([&string_found_tokens](std::string stringToken) {
            string_found_tokens.push_back(std::move(stringToken));
        });
        parser.parse(input);

        assertVector(integer_found_tokens, { 18446744073709551615ull });
        assertVector(string_found_tokens, { "18446744073709551616" });
    }
    std::cerr << "TestTokenParser is OK" << std::endl;
}

int main() {
    TestTokenParser();
    return 0;
}
