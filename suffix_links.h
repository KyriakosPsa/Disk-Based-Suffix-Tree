#include <string>
#include <unordered_map>
#include "node.h"
#include "suffix_tree.h"

std::pair<std::string, int> decodeSuffixlink(const std::string &suffixlink);

bool compareSubstring(Node &node, int &len_so_far, std::string &path);

int recursiveChildsearch(Node &node, int &len_so_far, std::string &path);

int parentNolinksearch(SuffixTree &tree, std::string &path);

int parentWithlink(SuffixTree &tree, std::string &path, int &parentlink);

int same_tree_parentNolinksearch(SuffixTree &tree, std::string &path, Node &currentnode, int currentnode_id);

void createSuffixlinks(std::string &directorypath);

void recursiveDFSearch(SuffixTree &tree, Node &childnode, std::string &path, std::string &parent_suffix_link, int &childnode_id);