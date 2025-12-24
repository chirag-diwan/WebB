#include "./Lexer.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

Lexer::Lexer(const char* HTMLPath) {
    std::ifstream file(HTMLPath);
    if (!file) {
        throw std::runtime_error("Failed to open HTML file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    input = std::stringstream(buffer.str());
}

int Lexer::Consume() {
    return input.get();
}

int Lexer::Peek() {
    return input.peek();
}

void Lexer::SkipWhitespace() {
    while (Peek() != EOF &&
           std::isspace(static_cast<unsigned char>(Peek()))) {
        Consume();
    }
}

void Lexer::Lex() {
    while (Peek() != EOF) {
        SkipWhitespace();
        int current = Peek();
        if (current == EOF) break;

        if (current == '<') {
            tokens.push_back({TokenType::ANGULAR_OPEN, std::string(1, Consume())});
        }
        else if (current == '>') {
            tokens.push_back({TokenType::ANGULAR_CLOSE, std::string(1, Consume())});
        }
        else if (current == '/') {
            tokens.push_back({TokenType::SLASH, std::string(1, Consume())});
        }
        else if (current == '=') {
            tokens.push_back({TokenType::EQUALS, std::string(1, Consume())});
        }
        else if (std::isalpha(static_cast<unsigned char>(current))) {
            std::string value;
            while (Peek() != EOF &&
                   std::isalpha(static_cast<unsigned char>(Peek()))) {
                value += Consume();
            }
            tokens.push_back({TokenType::ELEMENT, value});
        }
        else if (current == '!') {
            tokens.push_back({TokenType::NOT, std::string(1, Consume())});
        }
        else if (current == '"') {
            Consume(); // opening quote
            std::string value;
            while (Peek() != EOF && Peek() != '"') {
                value += Consume();
            }
            if (Peek() == '"') Consume(); // closing quote
            tokens.push_back({TokenType::PROPERTY, value});
        }
        else {
            std::string value;
            while (Peek() != EOF && Peek() != '<') {
                value += Consume();
            }
            if (!value.empty()) {
                tokens.push_back({TokenType::TEXT, value});
            }
        }
    }
}

std::vector<Token>& Lexer::GetTokens() {
    return tokens;
}

