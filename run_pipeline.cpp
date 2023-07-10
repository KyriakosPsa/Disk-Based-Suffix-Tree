#define NOMINMAX
#include "file_utils.h"
#include "prefix_count.h"
#include "suffix_tree.h"
#include "suffix_links.h"
#include "partitioning.h"
#include "node.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <filesystem>

void splitTrees(const std::vector<std::string> &prefixes, SuffixTree &tree, std::string &partitionName)
{
  std::ifstream ifs;
  std::vector<int> location;
  for (auto pre : prefixes)
  {
    if (!std::filesystem::is_directory("./temp_trees/" + pre) || !std::filesystem::exists("./temp_trees/" + pre))
    {
      std::filesystem::create_directory("./temp_trees/" + pre);
    }
  }

  for (const auto &pre : prefixes)
    {
      std::vector<int> location = tree.queryPrefix(pre);
      if (!(location.empty()))
      {
        SuffixTree newTree = *(splitTree(tree, pre, location));
        if (newTree.m_nodes.at(newTree.m_rootId).m_sub.length() > 0) {
          newTree.serialize("./temp_trees/" + pre + '/' + partitionName);
          std::string fName = "./temp_trees/" + pre + "/vis_" + partitionName;
          newTree.visualizeNoLeaves(fName);
        }
      }
      // need prefix location here.
    }
}

void constructTrees(const std::string &dir, const std::vector<std::string> &prefixes)
{
  std::string partition;
  if (!std::filesystem::is_directory("temp_trees") || !std::filesystem::exists("temp_trees"))
  {
    std::filesystem::create_directory("temp_trees");
  }

  if (!std::filesystem::is_directory("./temp_trees/partition_trees") || !std::filesystem::exists("./temp_trees/partition_trees"))
  {
    std::filesystem::create_directory("./temp_trees/partition_trees");
  }

  PartitionUtility pfr{};
  for (const auto &entry : std::filesystem::directory_iterator(dir))
  {
    std::string partitionName = entry.path().filename().string();
    partition = pfr.getPartitionString(partitionName);
    // std::cout << partition.length() << '\n';
    SuffixTree tree{partition};
    splitTrees(prefixes, tree, partitionName);
    // tree.serialize("./temp_trees/partition_trees/" + partitionName);
  }
}

void mergeTrees(const std::vector<std::string> &prefixes, size_t sequenceLength)
{
  if (!std::filesystem::is_directory("./temp_trees/merged_trees") || !std::filesystem::exists("./temp_trees/merged_trees"))
  {
    std::filesystem::create_directory("./temp_trees/merged_trees");
  }
  for (const auto &pre : prefixes)
  {
    std::filesystem::directory_iterator iter{"./temp_trees/" + pre};
    std::ifstream ifs;
    auto dir = *(iter);
    ifs.open(dir.path());
    SuffixTree baseTree{ifs};
    baseTree.splitRoot(pre);
    iter++;
    ifs.close();
    for (const auto &entry : iter)
    {
      ifs.open(entry.path());
      SuffixTree newTree{ifs};
      newTree.splitRoot(pre);
      baseTree.merge(newTree);
    }
    baseTree.m_length = sequenceLength;
    baseTree.makeLeaves();
    // std::cout << pre << " leaves: " << baseTree.m_leaves.size() << '\n';
    baseTree.serialize("./temp_trees/merged_trees/" + pre + ".txt");
    std::string fName = "./temp_trees/merged_trees/vis_" + pre + ".txt";
    baseTree.visualizeNoLeaves(fName);
    // std::ofstream save;
    // save.open("./temp_trees/merged_trees/" + pre + "_vis.txt");
    // baseTree.visualizeNoLeaves(save);
    // save.close();
    // baseTree.serialize("./temp_trees/merged_trees/" + pre + ".txt");
  }
}

void createSuffixlinks(std::vector<std::string> &prefixes)
{
  if (!std::filesystem::is_directory("./temp_trees/final_trees") || !std::filesystem::exists("./temp_trees/final_trees"))
  {
    std::filesystem::create_directory("./temp_trees/final_trees");
  }
  for (std::string prefix : prefixes)
  {
    std::ifstream ifs;
    ifs.open("./temp_trees/merged_trees/" + prefix + ".txt");
    SuffixTree tree{ifs};

    // Get the root node info
    int rootnodeid = tree.m_rootId;
    std::string rootnode_sub = tree.m_nodes.at(rootnodeid).m_sub;

    // Move to the children of the root node
    for (auto childid : tree.m_nodes.at(rootnodeid).m_children)
    {
      std::string suffix = (rootnode_sub + tree.m_nodes.at(childid).m_sub).substr(1);
      Node::SuffixLink parent_suffix_link = tree.m_nodes.at(tree.m_nodes.at(childid).m_parent).m_suffixLink;
      recursiveDFSearch(prefix, tree, tree.m_nodes.at(childid), suffix, parent_suffix_link, childid, prefixes);
      if (tree.m_nodes.at(childid).m_suffixLink.nodeId != -1)
      {
        // std::cout << std::to_string(childid) << std::endl;
        Node test = tree.m_nodes.at(childid);
      }
    }
    tree.serialize("./temp_trees/final_trees/" + prefix + ".txt");
    std::string fName = "./temp_trees/final_trees/vis_" + prefix + ".txt";
    tree.visualizeNoLeaves(fName);
    std::cout << "./temp_trees/final_trees/" + prefix + ".txt" << '\n';
  }
}

int main()
{
  std::string sequence = readSequence("input/medium.fasta");
  clearDir("temp_trees");
  clearDir("temp_prfx");
  // // Memory limits experimental section
  // // size_t minWorkingSetSize = 1024 * 1024 * 0.4;
  // // size_t maxWorkingSetSize = 1024 * 1024 * 0.4;
  // // MemoryUtil mu{};
  // // mu.limitMemory(minWorkingSetSize, maxWorkingSetSize);
  // // size_t t = mu.calculate_t(sequence.length(), maxWorkingSetSize);

  size_t t = sequence.length() / 30;
  // std::cout << t << '\n';
  PrefixCounter pc{static_cast<int>(t), sequence};
  std::vector<std::string> prefixes = pc.getPrefixes();
  std::cout << prefixes.size() << '\n';

  partitionSequence(sequence, t);
  std::cout << "Partitioning Done" << '\n';
  constructTrees("temp_prfx", prefixes);
  std::cout << "Partition Phase Done" << '\n';
  mergeTrees(prefixes, sequence.length());
  std::cout << "Merging Phase Done" << '\n';
  createSuffixlinks(prefixes);
  std::cout << "Suffix Link Phase Done" << '\n';
}

// Make list of variable length prefixes
// Partition sequence
// Create implicit suffix tree for each partition
// Make explicit by adding a unique prefix from the start of the next file
// Split trees by prefixes, taking only paths that begin with a given prefix
// Save prefix trees to disk