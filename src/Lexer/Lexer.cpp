#include "./Lexer.h"
#include <cctype>
#include <fstream>
#include <iostream>
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
  while (Peek() != EOF && std::isspace(static_cast<unsigned char>(Peek()))) {
    Consume();
  }
}

std::string Lexer::ConsumeUntil(char delimiter) {
  std::string value;
  while (Peek() != EOF && Peek() != delimiter) {
    value += Consume();
  }
  if (Peek() == delimiter) Consume();
  return value;
}

void Lexer::Lex() {
  while (Peek() != EOF) {
    SkipWhitespace();
    if (Peek() == EOF) break;

    if (Peek() == '<') {
      tokens.push_back({TokenType::ANGULAR_OPEN, std::string(1, Consume())});

      if (Peek() == '!') {
        Consume();  
        while(Peek() != '>' && Peek() != EOF) Consume();
        Consume();
        tokens.pop_back(); 
      }else if (Peek() == '/') {
        Consume();  
        tokens.push_back({TokenType::SLASH, "/"});

        SkipWhitespace();


        if (!std::isalpha(static_cast<unsigned char>(Peek()))) {


          while (Peek() != '>' && Peek() != EOF) Consume();
        } else {
          std::string value;
          while (Peek() != EOF &&
              (std::isalnum(static_cast<unsigned char>(Peek())) || 
               Peek() == '-' || Peek() == '_' || Peek() == ':')) {  
            value += Consume();
          }
          if (!value.empty()) {
            tokens.push_back({TokenType::ELEMENT, value});
          }
        }

        SkipWhitespace();

        if (Peek() == '>') {
          tokens.push_back({TokenType::ANGULAR_CLOSE, std::string(1, Consume())});
        } else {

          while (Peek() != '>' && Peek() != EOF) Consume();
          if (Peek() == '>') {
            tokens.push_back({TokenType::ANGULAR_CLOSE, std::string(1, Consume())});
          }
        }
      }else {

        while (Peek() != '>' && Peek() != EOF) {
          SkipWhitespace(); 

          int current = Peek();
          if (std::isalpha(static_cast<unsigned char>(current))) {

            std::string value;
            while (Peek() != EOF &&
                (std::isalnum(static_cast<unsigned char>(Peek())) || Peek() == '-' || Peek() == '_')) {
              value += Consume();
            }
            tokens.push_back({TokenType::ELEMENT, value});
          } else if (current == '=') {
            tokens.push_back({TokenType::EQUALS, std::string(1, Consume())});
          } else if (current == '"' || current == '\'') {

            char quote = Consume();
            std::string value = ConsumeUntil(quote);
            tokens.push_back({TokenType::PROPERTY, value});
          } else if (current == '/') {
            tokens.push_back({TokenType::SLASH, std::string(1, Consume())});
          } else if (std::isalnum(static_cast<unsigned char>(current))) {

            std::string value;
            while (Peek() != EOF && !std::isspace(static_cast<unsigned char>(Peek())) && Peek() != '>' && Peek() != '/') {
              value += Consume();
            }
            tokens.push_back({TokenType::PROPERTY, value});
          } else {
            Consume(); 
          }
        }
        if (Peek() == '>') {
          tokens.push_back({TokenType::ANGULAR_CLOSE, std::string(1, Consume())});
        }
      }
    } else {

      std::string value;
      while (Peek() != EOF && Peek() != '<') {
        value += Consume();
      }
      if (!value.empty()) {
        tokens.push_back({TokenType::TEXT, value});
      }
    }
  }
  tokens.push_back({TokenType::TOKEN_EOF, ""});
}

std::vector<Token>& Lexer::GetTokens() {
  return tokens;
}
