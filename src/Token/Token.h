#pragma once
#include <string>


enum class TokenType {
  ELEMENT,
  ANGULAR_OPEN,
  ANGULAR_CLOSE,
  SLASH,
  TEXT,
  PROPERTY,
  EQUALS,
  TOKEN_EOF,
  NOT
};

//Single Token 

struct Token {
  TokenType type;
  std::string value;
};
