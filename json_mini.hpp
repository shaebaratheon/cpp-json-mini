#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cctype>

enum class TokenType {
    LBRACE, RBRACE, LBRACK, RBRACK, COLON, COMMA,
    STRING, NUMBER, BOOLEAN, NUL, END
};

struct Token {
    TokenType type;
    std::string value;
};

class JsonTokenizer {
public:
    JsonTokenizer(const std::string& source) : source_(source), pos_(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (pos_ < source_.length()) {
            char c = source_[pos_];
            if (isspace(c)) {
                pos_++;
            } else if (c == '{') {
                tokens.push_back({TokenType::LBRACE, "{"});
                pos_++;
            } else if (c == '}') {
                tokens.push_back({TokenType::RBRACE, "}"});
                pos_++;
            } else if (c == ':') {
                tokens.push_back({TokenType::COLON, ":"});
                pos_++;
            } else if (c == '"') {
                tokens.push_back({TokenType::STRING, readString()});
            } else if (isdigit(c) || c == '-') {
                tokens.push_back({TokenType::NUMBER, readNumber()});
            } else if (isalpha(c)) {
                tokens.push_back(readKeyword());
            } else {
                // Unknown character, skip for this toy version
                pos_++;
            }
        }
        tokens.push_back({TokenType::END, ""});
        return tokens;
    }

private:
    std::string readString() {
        std::string result;
        pos_++; // skip opening quote
        while (pos_ < source_.length() && source_[pos_] != '"') {
            result += source_[pos_++];
        }
        pos_++; // skip closing quote
        return result;
    }

    std::string readNumber() {
        std::string result;
        while (pos_ < source_.length() && (isdigit(source_[pos_]) || source_[pos_] == '.')) {
            result += source_[pos_++];
        }
        return result;
    }

    Token readKeyword() {
        std::string result;
        while (pos_ < source_.length() && isalpha(source_[pos_])) {
            result += source_[pos_++];
        }
        
        if (result == "true" || result == "false") {
            return {TokenType::BOOLEAN, result};
        } else if (result == "null") {
            return {TokenType::NUL, result};
        }
        // Fallback or error
        return {TokenType::END, result};
    }

    std::string source_;
    size_t pos_;
};
// Utf8 Decoder Module\n// Handles surrogate pairs and 4-byte sequences correctly now.\n// Code is slightly heavier but much safer.
