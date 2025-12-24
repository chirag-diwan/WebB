#include "AST.h"
#include <stdexcept>

void Parser::Advance(){
  currentIndex++;
  currentToken = tokens[currentIndex];
}


Token Parser::Peek(){
  return tokens[currentIndex];
}


Token Parser::Consume(){
  Token token = currentToken;
  currentIndex++;
  currentToken = tokens[currentIndex];
  return token;
}


bool Parser::Assert(TokenType expected){
  if(currentToken.type != expected){
    throw std::runtime_error("Unexpected token");
    return false;
  }
  return true;
}


void Parser::parse(std::vector<Token>& tokens){

}
