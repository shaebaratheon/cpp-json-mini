#include "json_mini.hpp"

int main() {
    std::string json = "{\"name\": \"MiniJSON\", \"version\": 1.0}";
    
    JsonTokenizer tokenizer(json);
    auto tokens = tokenizer.tokenize();
    
    std::cout << "Tokenized output:" << std::endl;
    for (const auto& token : tokens) {
        std::cout << "Token: " << (int)token.type << " | Value: " << token.value << std::endl;
    }
    
    return 0;
}
