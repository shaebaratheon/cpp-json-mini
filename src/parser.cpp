#include "json_mini/parser.hpp"
#include <sstream>
#include <cctype>

namespace json_mini {

Parser::Parser(const std::string& input) : input_(input) {}

Value Parser::parse(const std::string& input) {
    Parser p(input);
    p.tokenize();
    Value v = p.parse_value();
    if (p.peek_token().type != LexerToken::EndOfFile) {
        throw std::runtime_error("Unexpected trailing tokens after JSON value");
    }
    return v;
}

void Parser::tokenize() {
    size_t pos = 0;
    size_t line = 1;
    size_t col = 1;

    while (pos < input_.size()) {
        char c = input_[pos];
        if (c == '\n') {
            line++;
            col = 1;
            pos++;
            continue;
        }
        if (std::isspace(static_cast<unsigned char>(c))) {
            col++;
            pos++;
            continue;
        }

        size_t start_col = col;
        if (c == '{') {
            tokens_.push_back({LexerToken::LeftBrace, "{", line, start_col});
            pos++; col++;
        } else if (c == '}') {
            tokens_.push_back({LexerToken::RightBrace, "}", line, start_col});
            pos++; col++;
        } else if (c == '[') {
            tokens_.push_back({LexerToken::LeftBracket, "[", line, start_col});
            pos++; col++;
        } else if (c == ']') {
            tokens_.push_back({LexerToken::RightBracket, "]", line, start_col});
            pos++; col++;
        } else if (c == ':') {
            tokens_.push_back({LexerToken::Colon, ":", line, start_col});
            pos++; col++;
        } else if (c == ',') {
            tokens_.push_back({LexerToken::Comma, ",", line, start_col});
            pos++; col++;
        } else if (c == '"') {
            pos++; col++;
            std::string str;
            bool escaped = false;
            while (pos < input_.size()) {
                char sc = input_[pos++];
                col++;
                if (escaped) {
                    str += '\\';
                    str += sc;
                    escaped = false;
                } else if (sc == '\\') {
                    escaped = true;
                } else if (sc == '"') {
                    break;
                } else {
                    str += sc;
                }
            }
            tokens_.push_back({LexerToken::String, str, line, start_col});
        } else if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
            std::string num;
            while (pos < input_.size()) {
                char nc = input_[pos];
                if (std::isdigit(static_cast<unsigned char>(nc)) || nc == '.' || nc == 'e' || nc == 'E' || nc == '+' || nc == '-') {
                    num += nc;
                    pos++;
                    col++;
                } else {
                    break;
                }
            }
            tokens_.push_back({LexerToken::Number, num, line, start_col});
        } else if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string ident;
            while (pos < input_.size() && std::isalpha(static_cast<unsigned char>(input_[pos]))) {
                ident += input_[pos++];
                col++;
            }
            if (ident == "true") {
                tokens_.push_back({LexerToken::True, "true", line, start_col});
            } else if (ident == "false") {
                tokens_.push_back({LexerToken::False, "false", line, start_col});
            } else if (ident == "null") {
                tokens_.push_back({LexerToken::Null, "null", line, start_col});
            } else {
                throw std::runtime_error("Unexpected token: " + ident + " at line " + std::to_string(line));
            }
        } else {
            throw std::runtime_error("Invalid character: " + std::string(1, c) + " at line " + std::to_string(line));
        }
    }
    tokens_.push_back({LexerToken::EndOfFile, "", line, col});
}

TokenItem Parser::next_token() {
    if (token_idx_ < tokens_.size()) {
        return tokens_[token_idx_++];
    }
    return tokens_.back();
}

const TokenItem& Parser::peek_token() const {
    if (token_idx_ < tokens_.size()) {
        return tokens_[token_idx_];
    }
    return tokens_.back();
}

bool Parser::match(LexerToken expected) {
    if (peek_token().type == expected) {
        token_idx_++;
        return true;
    }
    return false;
}

void Parser::expect(LexerToken expected, const std::string& msg) {
    if (!match(expected)) {
        const auto& tok = peek_token();
        throw std::runtime_error(msg + " at line " + std::to_string(tok.line) + ", col " + std::to_string(tok.col));
    }
}

std::string Parser::unescape_string(const std::string& str) {
    std::string out;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\\' && i + 1 < str.size()) {
            char next = str[++i];
            switch (next) {
                case '"': out += '"'; break;
                case '\\': out += '\\'; break;
                case '/': out += '/'; break;
                case 'b': out += '\b'; break;
                case 'f': out += '\f'; break;
                case 'n': out += '\n'; break;
                case 'r': out += '\r'; break;
                case 't': out += '\t'; break;
                default: out += next; break;
            }
        } else {
            out += str[i];
        }
    }
    return out;
}

Value Parser::parse_value() {
    const auto& tok = peek_token();
    switch (tok.type) {
        case LexerToken::LeftBrace:
            return parse_object();
        case LexerToken::LeftBracket:
            return parse_array();
        case LexerToken::String: {
            TokenItem item = next_token();
            return Value(unescape_string(item.text));
        }
        case LexerToken::Number: {
            TokenItem item = next_token();
            double d = std::stod(item.text);
            return Value(d);
        }
        case LexerToken::True:
            next_token();
            return Value(true);
        case LexerToken::False:
            next_token();
            return Value(false);
        case LexerToken::Null:
            next_token();
            return Value(nullptr);
        default:
            throw std::runtime_error("Unexpected token type when parsing value at line " + std::to_string(tok.line));
    }
}

Value Parser::parse_object() {
    expect(LexerToken::LeftBrace, "Expected '{'");
    Value::Object obj;
    if (match(LexerToken::RightBrace)) {
        return Value(obj);
    }

    while (true) {
        const auto& key_tok = peek_token();
        if (key_tok.type != LexerToken::String) {
            throw std::runtime_error("Object key must be a string at line " + std::to_string(key_tok.line));
        }
        std::string key = unescape_string(next_token().text);
        expect(LexerToken::Colon, "Expected ':' after object key");
        Value val = parse_value();
        obj[key] = std::move(val);

        if (match(LexerToken::RightBrace)) {
            break;
        }
        expect(LexerToken::Comma, "Expected ',' or '}' in object");
    }
    return Value(obj);
}

Value Parser::parse_array() {
    expect(LexerToken::LeftBracket, "Expected '['");
    Value::Array arr;
    if (match(LexerToken::RightBracket)) {
        return Value(arr);
    }

    while (true) {
        Value val = parse_value();
        arr.push_back(std::move(val));

        if (match(LexerToken::RightBracket)) {
            break;
        }
        expect(LexerToken::Comma, "Expected ',' or ']' in array");
    }
    return Value(arr);
}

} // namespace json_mini
