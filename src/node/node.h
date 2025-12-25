#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

enum class NodeType {
  Document,
  Element,
  Text,
  Comment
};



class Node {
  public:
    NodeType type;

    std::string name;   
    std::string text;   

    std::unordered_map<std::string, std::string> attributes;

    std::vector<std::unique_ptr<Node>> children;
    Node* parent = nullptr;
    bool selfClosing = false;
};
#include <iostream>
#include <string>


void PrintAST(const Node* node, int depth = 0);
