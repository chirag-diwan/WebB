#pragma once

#include <sstream>
#include <vector>
#include "../Token/Token.h"


class Lexer {
  private:
    std::stringstream input;
    std::vector<Token> tokens;
  
  private:
    int Consume();
    int Peek();
    void SkipWhitespace();
    std::string ConsumeUntil(char delimiter) ;

  public:
    Lexer(const char * HTMLPath);
    void Lex();
    std::vector<Token>& GetTokens();
};
