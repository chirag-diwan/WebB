#pragma once


#include <memory>
#include <vector>
#include "../Token/Token.h"
#include "../node/node.h"

class Parser {
  private:
    std::vector<Token> tokens;
    size_t currentIndex = 0;
    Token currentToken;
  private:
    void Advance();
    Token Peek();
    Token PeekNext();
    Token Consume();
    bool Assert(TokenType expected);
    bool Present(TokenType tokenType);
  public:
    Parser(std::vector<Token>& tokens) : tokens(tokens) {
      currentToken = tokens[0];
    }
    std::unique_ptr<Node> MakeAST();
};
