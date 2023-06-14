#ifndef SUFFIX_TREE
#define SUFFIX_TREE

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "node.h"
#include "utils.h"

class SuffixTree
{
public:
    std::unordered_map<int, Node> m_nodes{};
    std::unordered_map<int, int> m_leaves{}; // Key -> Node unique id , Value -> suffix index in the original string
    size_t m_length{0};
    int m_rootId{0};

    SuffixTree();

    SuffixTree(std::vector<std::pair<int, Node>> nodes, size_t length, int rootId, IdFactory *idFactory);

    SuffixTree(const std::string &str, IdFactory *idFactory, const std::string &prefix = "");

    SuffixTree(std::ifstream &archive);

    // void build(const std::string &str, const std::string &prefix = "");

    void makeExplicit(const std::string &str);

    bool isUnique(const std::string &str);

    void makeLeaves();

    void visualize();

    void visualizeNoLeaves();

    void visualizeNoLeaves(std::ofstream &outputFile);

    void getAllChildren(int node, std::vector<std::pair<int, Node>> &children);

    std::vector<int> queryPrefix(const std::string &str);

    void deleteNode(int node);

    void removeAsChild(int node);

    void deleteNode(std::vector<std::pair<int, Node>> nodes);

    size_t stringToSize(std::string &str);

    void serialize(const std::string &fileName);

    int countLeaves();

private:
    IdFactory *m_idFactory;
    void addSuffix(const std::string &suf);

    std::string serializeLeaves();

    std::unordered_map<int, int> deserializeLeaves(std::string &line);

    std::string serializeNodes();

    std::unordered_map<int, Node> deserializeNodes(std::string &line);
};

SuffixTree *splitTree(SuffixTree &tree, const std::string &str, std::vector<int> prefixLocation, IdFactory *idFactory);

#endif