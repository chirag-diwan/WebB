#pragma once


#include <vector>
#include "../Token/Token.h"


class Parser {
  private:
    std::vector<Token> tokens;
    size_t currentIndex = 0;
    Token currentToken;
  private:
    Parser(std::vector<Token>& tokens) : tokens(tokens) {
      currentToken = tokens[0];
    }
    void Advance();
    Token Peek();
    Token Consume();
    bool Assert(TokenType expected);
  public:
    void parse(std::vector<Token>& tokens);
};
