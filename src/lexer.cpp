#include "json_mini/lexer.hpp"
#include <cctype>

namespace json_mini {

Lexer::Lexer(std::string source) : source_(std::move(source)) {}

bool Lexer::is_at_end() const {
    return cursor_ >= source_.length();
}

char Lexer::peek() const {
    return is_at_end() ? '\0' : source_[cursor_];
}

char Lexer::advance() {
    char c = source_[cursor_++];
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return c;
}

void Lexer::skip_whitespace() {
    while (!is_at_end()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!is_at_end()) {
        skip_whitespace();
        if (is_at_end()) break;

        size_t start_line = line_;
        size_t start_col = column_;
        char c = peek();

        if (c == '{') { advance(); tokens.push_back({TokenType::LeftBrace, "{", start_line, start_col}); }
        else if (c == '}') { advance(); tokens.push_back({TokenType::RightBrace, "}", start_line, start_col}); }
        else if (c == '[') { advance(); tokens.push_back({TokenType::LeftBracket, "[", start_line, start_col}); }
        else if (c == ']') { advance(); tokens.push_back({TokenType::RightBracket, "]", start_line, start_col}); }
        else if (c == ':') { advance(); tokens.push_back({TokenType::Colon, ":", start_line, start_col}); }
        else if (c == ',') { advance(); tokens.push_back({TokenType::Comma, ",", start_line, start_col}); }
        else if (c == '"') { tokens.push_back(scan_string()); }
        else if (std::isdigit(c) || c == '-') { tokens.push_back(scan_number()); }
        else if (std::isalpha(c)) { tokens.push_back(scan_identifier()); }
        else {
            throw std::runtime_error(std::string("Unexpected character: ") + c);
        }
    }
    tokens.push_back({TokenType::EndOfFile, "", line_, column_});
    return tokens;
}

Token Lexer::scan_string() {
    size_t start_line = line_;
    size_t start_col = column_;
    advance(); // Consume open quote
    std::string val;
    while (!is_at_end() && peek() != '"') {
        if (peek() == '\\') {
            advance();
            char esc = advance();
            if (esc == 'n') val += '\n';
            else if (esc == 't') val += '\t';
            else if (esc == '"') val += '"';
            else if (esc == '\\') val += '\\';
            else val += esc;
        } else {
            val += advance();
        }
    }
    if (is_at_end()) throw std::runtime_error("Unterminated string literal");
    advance(); // Consume close quote
    return {TokenType::String, val, start_line, start_col};
}

Token Lexer::scan_number() {
    size_t start_line = line_;
    size_t start_col = column_;
    std::string val;
    if (peek() == '-') val += advance();
    while (std::isdigit(peek())) val += advance();
    if (peek() == '.') {
        val += advance();
        while (std::isdigit(peek())) val += advance();
    }
    return {TokenType::Number, val, start_line, start_col};
}

Token Lexer::scan_identifier() {
    size_t start_line = line_;
    size_t start_col = column_;
    std::string val;
    while (std::isalpha(peek())) val += advance();
    if (val == "true") return {TokenType::True, val, start_line, start_col};
    if (val == "false") return {TokenType::False, val, start_line, start_col};
    if (val == "null") return {TokenType::Null, val, start_line, start_col};
    throw std::runtime_error("Unknown identifier: " + val);
}

} // namespace json_mini
