#include <string>
#include <unordered_map>
#include "node.h"
#include "suffix_tree.h"

bool compareSubstring(Node &node, int &len_so_far, std::string &path);

int recursiveChildsearch(Node &node, int &len_so_far, std::string &path);

int parentNolinksearch(SuffixTree &tree, std::string &path);

int parentWithlink(SuffixTree &tree, std::string &path, int &parentlink);

int same_tree_parentNolinksearch(SuffixTree &tree, std::string &path, Node &currentnode, int currentnode_id);

void recursiveDFSearch(std::string &treeName, SuffixTree &tree, Node &childnode, std::string &path, Node::SuffixLink &parent_suffix_link, int &childnode_id, std::vector<std::string> &prefixes);

std::string getNodepathsofar(SuffixTree &tree, int linknode_id);