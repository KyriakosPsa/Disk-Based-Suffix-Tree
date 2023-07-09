#ifndef NODE
#define NODE

#include <string>
#include <vector>

class Node
{
public:
    std::string m_sub{""};
    std::vector<int> m_children{};
    int m_parent{-1};

    struct SuffixLink
    {
        std::string treeFile = "";
        int nodeId = -1;
    };

    SuffixLink m_suffixLink;

    SuffixLink decodeSuffixlink(const std::string &suffixlink);

    Node();

    Node(const std::string &sub, std::initializer_list<int> children, int parent);

    Node(const std::string &sub, std::vector<int> &children, int parent);

    Node(const std::string &str);

    std::string serialize();

    std::string serializeChildren();

    std::vector<int> deserializeChildren(std::string &str);
};

#endif