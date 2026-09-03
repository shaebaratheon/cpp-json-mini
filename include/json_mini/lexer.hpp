#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace json_mini {

enum class TokenType {
    LeftBrace,      // {
    RightBrace,     // }
    LeftBracket,    // [
    RightBracket,   // ]
    Colon,          // :
    Comma,          // ,
    String,         // "..."
    Number,         // 123, 45.67
    True,           // true
    False,          // false
    Null,           // null
    EndOfFile
};

struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
};

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    char peek() const;
    char advance();
    bool is_at_end() const;
    void skip_whitespace();
    Token scan_string();
    Token scan_number();
    Token scan_identifier();

    std::string source_;
    size_t cursor_ = 0;
    size_t line_ = 1;
    size_t column_ = 1;
};

} // namespace json_mini
