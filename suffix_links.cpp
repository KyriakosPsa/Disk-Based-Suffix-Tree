#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "node.h"
#include "suffix_links.h"
#include "suffix_tree.h"
#include <iostream>
#include <filesystem>

// Helper function to decode suffix links
std::pair<std::string, int> decodeSuffixlink(const std::string &suffixlink)
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
  return std::make_pair(tree, std::stoi(number));
}

// Helper function to compare the substring of a node to the path
bool compareSubstring(Node &node, size_t &len_so_far, std::string &path)
{
  std::string currentnodesub = node.m_sub;
  int result = path.substr(len_so_far).rfind(currentnodesub, 0);
  if (result == 0)
  {
    len_so_far += currentnodesub.length();
    return true;
  }
  else
  {
    return false;
  }
}

// Helper function to recursively search the children of a node
int recursiveChildsearch(Node &node, size_t &len_so_far, std::string &path, SuffixTree &trees)
{
  for (auto childid : node.m_children)
  {
    Node currentnode = trees.m_nodes.at(childid);
    bool result = compareSubstring(currentnode, len_so_far, path);
    if ((result) && (len_so_far == path.length()))
    {
      return childid;
    }
    else if (result)
    {
      recursiveChildsearch(currentnode, len_so_far, path, trees);
    }
    else
    {
      continue;
    }
  }
  return -1;
}

// Returns the node id of the node that matches the path in the tree, -1 otherwise
int parentNolinksearch(SuffixTree &tree, std::string &path)
{
  size_t len_so_far = 0;
  Node currentnode = tree.m_nodes.at(tree.m_rootId);
  bool result = compareSubstring(currentnode, len_so_far, path);
  if (result)
  {
    if (len_so_far == path.length())
    {
      return tree.m_rootId;
    }
    else
    {
      int node_id = recursiveChildsearch(currentnode, len_so_far, path, tree);
      return node_id;
    }
  }
  return -1;
}

int same_tree_parentNolinksearch(SuffixTree &tree, std::string &path, Node &currentnode, int currentnode_id)
{
  size_t len_so_far = 0;
  int parentid = currentnode.m_parent;
  Node parentnode = tree.m_nodes.at(parentid);
  for (auto childid : parentnode.m_children)
  {
    if (currentnode_id == childid)
    {
      continue;
    }
    Node currentnode = tree.m_nodes.at(childid);
    bool result = compareSubstring(currentnode, len_so_far, path);
    if (result)
    {
      if (len_so_far == path.length())
      {
        return childid;
      }
      else
      {
        int node_id = recursiveChildsearch(currentnode, len_so_far, path, tree);
        return node_id;
      }
    }
    return -1;
  }
  return -1;
}

int parentWithlink(SuffixTree &tree, std::string &path, int &parentlink)
{
  Node currentnode = tree.m_nodes.at(parentlink);
  size_t len_so_far = 0;
  int node_id = recursiveChildsearch(currentnode, len_so_far, path, tree);
  return node_id;
}

// Recursive Depth First Search to find the suffix links of the nodes in the tree
void recursiveDFSearch(std::string &treeName, SuffixTree &tree, Node &childnode, std::string &path, std::string &parent_suffix_link, int &childnode_id)
{
  // Case 1 parent has link
  if (parent_suffix_link != "")
  {
    std::pair<std::string, int> linkinfo = decodeSuffixlink(parent_suffix_link);
    std::string linkedtree = linkinfo.first;
    std::ifstream ifs;
    ifs.open("temp_trees/merged_trees/" + linkedtree);
    SuffixTree parenttree{ifs};
    int link = parentWithlink(parenttree, path, linkinfo.second);
    if (link != -1)
    {
      std ::cout << "parent with link works" << std::endl;
      childnode.m_suffixLink = linkedtree + "*" + std::to_string(link);
    }
  }

  // Case 2 parent has no link
  else
  {
    // Sub case 1: a link for the current node exist in the current tree
    int link = same_tree_parentNolinksearch(tree, path, childnode, childnode_id);
    if (link != -1)
    {
      childnode.m_suffixLink = treeName + "*" + std::to_string(link);
    }
    // Sub case 2: a link for the current node exist in another tree
    else
    {
      for (const auto &entry : std::filesystem::directory_iterator("temp_trees/merged_trees"))
      {
        std::string othertreename = entry.path().filename().string();
        if (tree.m_nodes.at(tree.m_rootId).m_sub + ".txt" == othertreename)
        {
          continue;
        }
        std::ifstream ifs;
        ifs.open(entry.path().string());
        SuffixTree othertree{ifs};
        int link = parentNolinksearch(othertree, path);
        if (link != -1)
        { // Output for testing
          std::cout << "Other tree without parent works" << std::endl;
          childnode.m_suffixLink = othertreename + "*" + std::to_string(link);
        }
      }
    }
  }
  for (int childid : childnode.m_children)
  {
    parent_suffix_link = childnode.m_suffixLink;
    if (childid == 0)
    {
      std::cout << 0 << std::endl;
    }
    path = (childnode.m_sub + tree.m_nodes.at(childid).m_sub).substr(1);
    Node childnode_new = tree.m_nodes.at(childid);
    childnode_id = childid;
    recursiveDFSearch(treeName, tree, childnode_new, path, parent_suffix_link, childnode_id);
  }
}

// MAIN REFACTORING //////////////////////////////////////////////////////////////
void createSuffixlinks(std::string &directorypath)
{
  for (const auto &entry : std::filesystem::directory_iterator(directorypath))
  {
    std::string current_tree = entry.path().filename().string();
    std::ifstream ifs;
    ifs.open(entry.path().string());
    SuffixTree tree{ifs};

    // Get the root node info
    int rootnodeid = tree.m_rootId;
    std::string rootnode_sub = tree.m_nodes.at(rootnodeid).m_sub;

    // Move to the children of the root node
    for (auto childid : tree.m_nodes.at(rootnodeid).m_children)
    {
      std::string path = rootnode_sub + tree.m_nodes.at(childid).m_sub;
      Node childnode = tree.m_nodes.at(childid);
      std::string suffix = (rootnode_sub + childnode.m_sub).substr(1);
      std::string parent_suffix_link = tree.m_nodes.at(childnode.m_parent).m_suffixLink;
      recursiveDFSearch(current_tree, tree, childnode, suffix, parent_suffix_link, childid);
    }
    std::cout << "Suffix links created for " << current_tree << ":')" << std::endl;
  }
}
