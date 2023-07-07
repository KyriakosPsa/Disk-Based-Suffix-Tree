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
  size_t currentnodesublen = currentnodesub.length();
  size_t pathlen = path.length();
  for (size_t i = 0; i < currentnodesublen; i++)
  {
    if ((i + len_so_far < pathlen) && (currentnodesub[i] == path[i + len_so_far]))
    {
      len_so_far++;
    }
    else
    {
      return false;
    }
  }
  return true;
}

// Helper function to recursively search the children of a node
int recursiveChildsearch(Node &node, size_t &len_so_far, std::string &path, SuffixTree &trees)
{
  for (auto childid : node.m_children)
  {
    Node currentnode = trees.m_nodes.at(childid);
    int result = compareSubstring(currentnode, len_so_far, path);
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

// Returns the node id of the node that matches the path in the tree for nodes that have parents
// with suffix links, -1 otherwise
int parentWithlink(SuffixTree &tree, std::string &path, int &parentlink)
{
  Node currentnode = tree.m_nodes.at(parentlink);
  size_t len_so_far = 0;
  int node_id = recursiveChildsearch(currentnode, len_so_far, path, tree);
  return node_id;
}
// Function to create suffix links for every node in a directory of trees using DFS traversal
void createSuffixlinks(std::string &directorypath)
{
  for (const auto &entry : std::filesystem::directory_iterator(directorypath))
  {
    // Open the tree
    std::string current_tree = entry.path().filename().string();
    std::ifstream ifs;
    ifs.open(entry.path().string());
    SuffixTree tree{ifs};

    // VISUALIZATION FOR TESTING
    tree.visualizeNoLeaves();

    // We start in a depth first search from the children of the root
    int rootnodeid = tree.m_rootId;
    std::string rootnode_sub = tree.m_nodes.at(rootnodeid).m_sub;
    ///////// The following code creates the suffix links for the children of the root node //////////////////////////////////////////
    // Iterate over all the other children of the root node to find a node with the suffix string A
    for (auto childid : tree.m_nodes.at(rootnodeid).m_children)
    {
      // If the child node is the current node, skip it
      int currentChild = childid;
      Node &node = tree.m_nodes.at(childid);
      std::string path = (rootnode_sub + node.m_sub).substr(1);
      if (tree.m_nodes.at(childid).m_sub == path)
      {
        node.m_suffixLink = current_tree + "*" + std::to_string(childid);
      }
      else
      {
        for (const auto &entry : std::filesystem::directory_iterator(directorypath))
        {
          std::string otherTree_name = entry.path().filename().string();
          std::string other_tree_path = entry.path().string();
          std::ifstream ifs_other;
          ifs_other.open(other_tree_path);
          SuffixTree other_tree{ifs_other};
          {
            int link = parentNolinksearch(other_tree, path);
            if (link != -1)
            {
              node.m_suffixLink = otherTree_name + "*" + std::to_string(link);
              // Output for testing
              std::cout << "The suffix link for node " << currentChild << " is " << node.m_suffixLink << std::endl;
              std::cout << "TREE COMPLETE" << std::endl;
              break;
            }
          }
        }
        while (true)
        {
          for (auto childid : tree.m_nodes.at(currentChild).m_children)
          {
            Node &node = tree.m_nodes.at(childid);
            int parent = node.m_parent;
            path = tree.m_nodes.at(childid).m_sub;
            std::pair<std::string, int> parent_link = decodeSuffixlink(tree.m_nodes.at(parent).m_suffixLink);
            int node_id = parentWithlink(tree, path, parent_link.second);
            if (node_id != -1)
            {
              node.m_suffixLink = parent_link.first + "*" + std::to_string(parent_link.second);
              break;
            }
          }
        }
      }
    }
  }
}
