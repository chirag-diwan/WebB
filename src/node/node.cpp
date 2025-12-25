#include <iostream>
#include <string>
#include "node.h"



void PrintAST(const Node* node, int depth) {
  if (!node) return;

  std::string indent(depth * 2, ' '); 

  if (node->type == NodeType::Document) {
    std::cout << indent << "<Document>\n";
  } else if (node->type == NodeType::Element) {
    std::cout << indent << "<" << node->name;


    for (const auto& [name, value] : node->attributes) {
      if (value.empty()) {
        std::cout << " " << name; 
      } else {
        std::cout << " " << name << "=\"" << value << "\"";
      }
    }

    if (node->selfClosing) {
      std::cout << " />\n";
    } else {
      std::cout << ">\n";
    }
  } else if (node->type == NodeType::Text) {

    std::string text = node->text;
    if (!text.empty() && text.find_first_not_of(" \t\n\r") != std::string::npos) {
      std::cout << indent << "  [Text: \"" << text << "\"]\n";
    } else {
      std::cout << indent << "  [Whitespace]\n";
    }
  }


  for (const auto& child : node->children) {
    PrintAST(child.get(), depth + 1);
  }


  if (node->type == NodeType::Element && !node->selfClosing) {
    std::cout << indent << "</" << node->name << ">\n";
  }
}
