#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "node.h"
#include "suffix_links.h"
#include "suffix_tree.h"
#include <iostream>
#include <filesystem>

// Helper function to produce the path of a node to the root
std::string getNodepathsofar(SuffixTree &tree, int linknode_id)
{
  Node node = tree.m_nodes.at(linknode_id);
  std::string pathsofar = node.m_sub;
  int parentid = node.m_parent;
  while (parentid != -1)
  {
    Node parentnode = tree.m_nodes.at(parentid);
    pathsofar = parentnode.m_sub + pathsofar;
    parentid = parentnode.m_parent;
  }
  return pathsofar;
}

// Helper function to compare the substring of a node to the path
int compareSubstring(Node &node, int &len_so_far, std::string &path)
{
  std::string currentnodesub = node.m_sub;
  int result = path.substr(static_cast<size_t>(len_so_far)).rfind(currentnodesub, 0);
  if (result == 0)
  {
    return currentnodesub.length();
  }
  else
  {
    return -1;
  }
}

// Helper function to recursively search the children of a node
int recursiveChildsearch(Node &node, int &len_so_far, std::string &path, SuffixTree &trees)
{
  for (auto childid : node.m_children)
  {
    Node currentnode = trees.m_nodes.at(childid);
    int result = compareSubstring(currentnode, len_so_far, path);
    if ((result > 0))
    {
      len_so_far += result;
      if (len_so_far == static_cast<int>(path.length())) {
        return childid;
      } else {
        int childOfChildId = recursiveChildsearch(currentnode, len_so_far, path, trees);
        if (childOfChildId > 0) {
          return childOfChildId;
        } else {
          len_so_far -= result;
        }
      }
    }
  }
  return -1;
}

// Returns the node id of the node that matches the path in the tree, -1 otherwise
int parentNolinksearch(SuffixTree &tree, std::string &path)
{
  int len_so_far = 0;
  Node currentnode = tree.m_nodes.at(tree.m_rootId);
  int result = compareSubstring(currentnode, len_so_far, path);
  if (result > 0)
  {
    len_so_far += result;
    if (len_so_far == static_cast<int>(path.length()))
    {
      return tree.m_rootId;
    }
    else
    {
        int childOfChildId = recursiveChildsearch(currentnode, len_so_far, path, tree);
        if (childOfChildId > 0) {
          return childOfChildId;
        } else {
          len_so_far -= result;
        }
    }
  }
  return -1;
}

int same_tree_parentNolinksearch(SuffixTree &tree, std::string &path, Node &currentnode, int currentnode_id)
{
  int len_so_far = 0;
  int parentid = currentnode.m_parent;
  Node parentnode = tree.m_nodes.at(parentid);
  for (auto childid : parentnode.m_children)
  {
    if (currentnode_id == childid)
    {
      continue;
    }
    Node currentnode = tree.m_nodes.at(childid);
    int result = compareSubstring(currentnode, len_so_far, path);
    if (result > 0)
    {
      len_so_far += result;
      if (len_so_far == static_cast<int>(path.length()))
      {
        return childid;
      }
      else
      {
        int node_id = recursiveChildsearch(currentnode, len_so_far, path, tree);
        if (node_id > 0) {
          return node_id;
        } else {
          len_so_far -= result;
        }
      }
    }
  }
  return -1;
}

int parentWithlink(SuffixTree &tree, std::string &path, int &parentlink)
{
  Node currentnode = tree.m_nodes.at(parentlink);
  // TODO
  int len_so_far = 0;
  int node_id = recursiveChildsearch(currentnode, len_so_far, path, tree);
  return node_id;
}

// Recursive Depth First Search to find the suffix links of the nodes in the tree
void recursiveDFSearch(std::string &treeName, SuffixTree &tree, Node &childnode, std::string &path, Node::SuffixLink &parent_suffix_link, int &childnode_id, std::vector<std::string> &prefixes)
{
  // Case 1 parent has link
  if (parent_suffix_link.nodeId != -1)
  {
    std::string linkedtree = parent_suffix_link.treeFile;
    std::ifstream ifs;
    ifs.open("temp_trees/merged_trees/" + linkedtree + ".txt");
    SuffixTree parenttree{ifs};
    int link = parentWithlink(parenttree, path, parent_suffix_link.nodeId);
    if (link != -1)
    {
      childnode.m_suffixLink.treeFile = linkedtree;
      childnode.m_suffixLink.nodeId = link;

      // OUTPUT FOR TESTING
      std ::cout << "\nparent with link works" << std::endl;
      std::cout << "link: for path " << path << " of node " << std::to_string(childnode_id) << " is "
                << "node " << std::to_string(link) << " with sub: " << parenttree.m_nodes.at(link).m_sub << std::endl;
      std::cout << "Reporting link node path:" << std::endl;
      std::string linkpath = getNodepathsofar(parenttree, link);
      std::cout << linkpath << std::endl;
    }
  }

  // Case 2 parent has no link
  else
  {
    // Sub case 1: a link for the current node exist in the current tree
    int link = same_tree_parentNolinksearch(tree, path, childnode, childnode_id);
    if (link != -1)
    {
      childnode.m_suffixLink.treeFile = treeName;
      childnode.m_suffixLink.nodeId = link;

      // OUTPUT FOR TESTING
      std::cout << "\nsame tree without parent works" << std::endl;
      std ::cout << "parent with link works" << std::endl;
      std::cout << "link: for path " << path << " of node " << std::to_string(childnode_id) << " is "
                << "node " << std::to_string(link) << " with sub: " << tree.m_nodes.at(link).m_sub << std::endl;
      std::cout << "Reporting link node path:" << std::endl;
      std::string linkpath = getNodepathsofar(tree, link);
      std::cout << linkpath << std::endl;
    }
    // Sub case 2: a link for the current node exist in another tree
    else
    {
      std::string correctTree;
      for (std::string prefix : prefixes)
      {
        if (path < prefix)
        {
          continue;
        }
        else if (path >= prefix)
        {
          int result = path.rfind(prefix, 0);
          if (result == 0)
          {
            correctTree = prefix;
          }
        }
      }
      if (correctTree == "")
      {
        return;
      }
      std::ifstream ifs;
      ifs.open("./temp_trees/merged_trees/" + correctTree + ".txt");
      SuffixTree othertree{ifs};
      int link = parentNolinksearch(othertree, path);
      if (link != -1)
      {
        childnode.m_suffixLink.treeFile = correctTree;
        childnode.m_suffixLink.nodeId = link;

        // Output for testing
        std::cout << "\nOther tree without parent works" << std::endl;
        std::cout << "link: for path " << path << " of node " << std::to_string(childnode_id) << " is "
                  << "node " << std::to_string(link) << " with sub: " << othertree.m_nodes.at(link).m_sub << std::endl;
        std::cout << "Reporting link node path:" << std::endl;
        std::string linkpath = getNodepathsofar(othertree, link);
        std::cout << linkpath << std::endl;
      }
    }
  }
  for (int childid : childnode.m_children)
  {
    std::string childpath = path + tree.m_nodes.at(childid).m_sub;
    parent_suffix_link = childnode.m_suffixLink;
    // path += tree.m_nodes.at(childid).m_sub;
    recursiveDFSearch(treeName, tree, tree.m_nodes.at(childid), childpath, parent_suffix_link, childid, prefixes);
  }
}
