#include "./Lexer/Lexer.h"
#include "./AST/AST.h"
#include <iostream>

int main() {
  Lexer lexer("example.html");
  lexer.Lex();
  auto x = lexer.GetTokens();
  for(auto& token : x) {
    std::cout << "Token Type: " << static_cast<int>(token.type) << ", Value: " << token.value << std::endl;
  }
  Parser parser(x);
  auto ast = parser.MakeAST();

  std::cout << "Parsed AST:\n";
  PrintAST(ast.get());

  return 0;
}
