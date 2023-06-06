#include <vector>
#include <random>
#include <string>
#include <sstream>
#include "node.h"

using namespace std;

std::vector<std::string> splitByDelimiter(const std::string &str, char delimiter);

Node::Node()
{
    //  The node is initialized  empty.
}

// Constructor for Node that takes a substring and a list of child nodes as well a starting index.
Node::Node(const string &sub, initializer_list<int> children, int parent) : m_sub(sub), m_parent(parent)
{
    // The child nodes of the node are inserted into the end of the children vector.
    m_children.insert(m_children.end(), children);
}

Node::Node(const string &sub, vector<int> &children, int parent) : m_sub(sub), m_parent(parent)
{
    // The child nodes of the node are inserted into the end of the children vector.
    m_children.insert(m_children.end(), children.begin(), children.end());
}

Node::Node(const string &str)
{
    std::vector<std::string> segments = splitByDelimiter(str, ',');;
    m_sub = segments[0];
    m_children = deserializeChildren(segments[1]);
    m_parent = stoi(segments[2]);
}

std::string Node::serialize()
{
    std::string gen{""};
    gen += m_sub;
    gen += ',';
    gen += serializeChildren();
    gen += ',';
    gen += to_string(m_parent);
    return gen;
}

std::string Node::serializeChildren() {
    std::string temp{""};
    for (size_t i = 0; i < m_children.size(); i++) {
        temp += to_string(m_children[i]);
        if (i != m_children.size() - 1) {
            temp += '_';
        }
    
    }
    return temp;
}

std::vector<int> Node::deserializeChildren(std::string &str) {
    std::vector<std::string> segments = splitByDelimiter(str, '_');
    std::vector<int> children{};
    for (auto seg: segments) {
        children.push_back(stoi(seg));
    }
    return children;
}