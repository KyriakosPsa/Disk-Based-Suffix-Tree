#include <vector>
#include <random>
#include <string>
#include <sstream>
#include "node.h"

std::vector<std::string> splitByDelimiter(const std::string &str, char delimiter);

Node::Node()
{
    //  The node is initialized  empty.
}

// Constructor for Node that takes a substring and a list of child nodes as well a starting index.
Node::Node(const std::string &sub, std::initializer_list<int> children, int parent) : m_sub(sub), m_parent(parent)
{
    // The child nodes of the node are inserted into the end of the children vector.
    m_children.insert(m_children.end(), children);
}

Node::Node(const std::string &sub, std::vector<int> &children, int parent) : m_sub(sub), m_parent(parent)
{
    // The child nodes of the node are inserted into the end of the children vector.
    m_children.insert(m_children.end(), children.begin(), children.end());
}

Node::Node(const std::string &str)
{
    std::vector<std::string> segments = splitByDelimiter(str, ',');
    ;
    m_sub = segments[0];
    m_children = deserializeChildren(segments[1]);
    m_parent = stoi(segments[2]);
    m_suffixLink = decodeSuffixlink(segments[3]);
}

std::string Node::serialize()
{
    std::string gen{""};
    gen += m_sub;
    gen += ',';
    gen += serializeChildren();
    gen += ',';
    gen += std::to_string(m_parent);
    gen += ',';
    gen += m_suffixLink.treeFile + '*' + std::to_string(m_suffixLink.nodeId);
    return gen;
}

std::string Node::serializeChildren()
{
    std::string temp{""};
    for (size_t i = 0; i < m_children.size(); i++)
    {
        temp += std::to_string(m_children[i]);
        if (i != m_children.size() - 1)
        {
            temp += '_';
        }
    }
    return temp;
}

std::vector<int> Node::deserializeChildren(std::string &str)
{
    std::vector<std::string> segments = splitByDelimiter(str, '_');
    std::vector<int> children{};
    for (auto seg : segments)
    {
        children.push_back(stoi(seg));
    }
    return children;
}

Node::SuffixLink Node::decodeSuffixlink(const std::string &suffixlink)
{
    std::string number;
    std::string tree;
    bool foundAsterisk = false;
    for (char c : suffixlink)
    {
        if (c == '*')
        {
            foundAsterisk = true;
        }
        else if (foundAsterisk)
        {
            number += c;
        }
        else
        {
            tree += c;
        }
    }
    return Node::SuffixLink{tree, stoi(number)};
}
