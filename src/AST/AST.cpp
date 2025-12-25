#include "AST.h"
#include <memory>
#include <stdexcept>
#include <stack>
#include <unordered_set>



void Parser::Advance() {
  currentIndex++;
  if (currentIndex < tokens.size()) {
    currentToken = tokens[currentIndex];
  } else {
    currentToken = {TokenType::TOKEN_EOF, ""};
  }
}


Token Parser::Peek() {
  if (currentIndex + 1 < tokens.size()) {
    return tokens[currentIndex + 1];
  }
  return {TokenType::TOKEN_EOF, ""};
}


Token Parser::PeekNext() {  
  if (currentIndex + 2 < tokens.size()) {
    return tokens[currentIndex + 2];
  }
  return {TokenType::TOKEN_EOF, ""};
}


Token Parser::Consume() {
  Token token = currentToken;
  Advance();
  return token;
}


bool Parser::Assert(TokenType expected) {
  if (currentToken.type != expected) {
    throw std::runtime_error("Unexpected token: '" + currentToken.value +
        "' (expected " + std::to_string(static_cast<int>(expected)) +
        ") at index: " + std::to_string(currentIndex));
  }
  return true;
}


bool Parser::Present(TokenType tokenType) {
  return currentToken.type == tokenType;
}


std::unique_ptr<Node> Parser::MakeAST() {
  std::unordered_set<std::string> voidElements = {
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "param", "source", "track", "wbr"  
  };
  currentIndex = 0;
  if (!tokens.empty()) {
    currentToken = tokens[0];
  } else {
    currentToken = {TokenType::TOKEN_EOF, ""};
  }
  auto root = std::make_unique<Node>();
  root->type = NodeType::Document;
  std::stack<Node*> nodeStack;
  nodeStack.push(root.get());
  while (!Present(TokenType::TOKEN_EOF)) {
    if (Present(TokenType::ANGULAR_OPEN)) {
      Consume();
      if (Present(TokenType::SLASH)) {
        Consume();
        if (currentToken.type != TokenType::ELEMENT) {
          throw std::runtime_error("Expected tag name in closing tag at index: " + std::to_string(currentIndex));
        }
        std::string closingName = currentToken.value;
        Consume();
        Assert(TokenType::ANGULAR_CLOSE);
        Consume();
        if (nodeStack.size() <= 1 || nodeStack.top()->name != closingName) {
          throw std::runtime_error("Mismatched closing tag: </" + closingName + "> at index: " + std::to_string(currentIndex));
        }
        nodeStack.pop();
      } else {
        if (currentToken.type != TokenType::ELEMENT) {
          throw std::runtime_error("Expected tag name after '<' at index: " + std::to_string(currentIndex));
        }
        std::string tagName = currentToken.value;
        Consume();
        auto elementNode = std::make_unique<Node>();
        elementNode->type = NodeType::Element;
        elementNode->name = tagName;

        bool isVoid = voidElements.count(tagName) > 0;
        bool selfClosing = false;

        elementNode->selfClosing = selfClosing || isVoid;

        while (!Present(TokenType::ANGULAR_CLOSE) && !Present(TokenType::SLASH)) {
          if (Present(TokenType::ELEMENT)) {
            std::string attrName = currentToken.value;
            Consume();
            if (Present(TokenType::EQUALS)) {
              Consume();
              if (Present(TokenType::PROPERTY)) {
                elementNode->attributes[attrName] = currentToken.value;
                Consume();
              } else {
                elementNode->attributes[attrName] = "";
              }
            } else {
              elementNode->attributes[attrName] = "";
            }
          } else {
            Advance();
          }
        }


        selfClosing = false;
        if (Present(TokenType::SLASH)) {
          selfClosing = true;
          Consume();
        }
        Assert(TokenType::ANGULAR_CLOSE);
        Consume();
        elementNode->selfClosing = selfClosing;  

        elementNode->selfClosing = elementNode->selfClosing || isVoid;

        elementNode->parent = nodeStack.top();
        nodeStack.top()->children.push_back(std::move(elementNode));

        Node* added = nodeStack.top()->children.back().get();
        if (!added->selfClosing) {
          nodeStack.push(added);
        }
      }
    } else if (Present(TokenType::TEXT)) {
      auto textNode = std::make_unique<Node>();
      textNode->type = NodeType::Text;
      textNode->text = currentToken.value;
      textNode->parent = nodeStack.top();
      nodeStack.top()->children.push_back(std::move(textNode));
      Consume();
    } else {
      Advance();
    }
  }
  if (nodeStack.size() != 1) {
    throw std::runtime_error("Unclosed tags remaining on stack");
  }
  return root;
}
