#pragma once
#include "json_mini/value.hpp"
#include <string>
#include <vector>

namespace json_mini {

enum class LexerToken {
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    Colon,
    Comma,
    String,
    Number,
    True,
    False,
    Null,
    EndOfFile
};

struct TokenItem {
    LexerToken type;
    std::string text;
    size_t line;
    size_t col;
};

class Parser {
public:
    static Value parse(const std::string& input);

private:
    explicit Parser(const std::string& input);

    void tokenize();
    TokenItem next_token();
    const TokenItem& peek_token() const;
    bool match(LexerToken expected);
    void expect(LexerToken expected, const std::string& msg);

    Value parse_value();
    Value parse_object();
    Value parse_array();

    std::string unescape_string(const std::string& str);

    std::string input_;
    std::vector<TokenItem> tokens_;
    size_t token_idx_ = 0;
};

} // namespace json_mini
