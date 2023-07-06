#define NOMINMAX
#include "file_utils.h"
#include "prefix_count.h"
#include "suffix_tree.h"
#include "partitioning.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>

void constructTrees(const std::string &dir, IdFactory *idFactory, int partitions) {
  std::string partition;
  if (!std::filesystem::is_directory("temp_trees") || !std::filesystem::exists("temp_trees")) {
    std::filesystem::create_directory("temp_trees");
  }

  if (!std::filesystem::is_directory("./temp_trees/partition_trees") || !std::filesystem::exists("./temp_trees/partition_trees")) {
    std::filesystem::create_directory("./temp_trees/partition_trees");
  }

  PartitionUtility pfr{partitions};
  for (const auto& entry : std::filesystem::directory_iterator(dir)) {
    partition = pfr.getNextPartitionString();
    SuffixTree tree{partition, idFactory};
  
    // bool implicit{true};
    // std::string candidatePrefix;
    // // if this is the final partition, tree is already implicit
    // if (pfr.m_finalPartition) {
    //   implicit = false;
    // }
    // while (implicit) {
    //   candidatePrefix += pfr.getAdditionalCharacter();
    //   bool unique = tree.isUnique(candidatePrefix);
    //   if (unique) {
    //     std::cout << candidatePrefix << '\n';
    //     tree.makeExplicit(candidatePrefix);
    //     implicit = false;
    //   }
    // }
    std::string partitionName = entry.path().filename().string();
    tree.serialize("./temp_trees/partition_trees/" + partitionName);
    // std::cout << partitionName << '\n';
  }
}

void splitTrees(const std::vector<std::string> &prefixes, IdFactory *idFactory) {
  std::ifstream ifs;
  std::vector<int> location;
  for (auto pre: prefixes) {
    if (!std::filesystem::is_directory("./temp_trees/" + pre) || !std::filesystem::exists("./temp_trees/" + pre)) {
      std::filesystem::create_directory("./temp_trees/" + pre);
    }
  }

  for (const auto& entry : std::filesystem::directory_iterator("./temp_trees/partition_trees/")) {
      ifs.open(entry.path());
      SuffixTree tree{ifs, idFactory};
      for (const auto& pre: prefixes) {
        std::vector<int> location = tree.queryPrefix(pre);
        if (!(location.empty()))
        {
            SuffixTree newTree = *(splitTree(tree, pre, location, idFactory));
            newTree.serialize("./temp_trees/" + pre + '/' + entry.path().filename().string());
        }
        // need prefix location here.
    }
    }
}

void mergeTrees(const std::vector<std::string> &prefixes, IdFactory* idFactory) {
  if (!std::filesystem::is_directory("./temp_trees/merged_trees") || !std::filesystem::exists("./temp_trees/merged_trees")) {
      std::filesystem::create_directory("./temp_trees/merged_trees");
    }
  for (const auto& pre: prefixes) {
    std::filesystem::directory_iterator iter{"./temp_trees/" + pre};
    std::ifstream ifs;
    auto dir = *(iter);
    ifs.open(dir.path());
    SuffixTree baseTree{ifs, idFactory};
    baseTree.splitRoot(pre);
    iter++;
    ifs.close();
    for (const auto& entry : iter) {
      ifs.open(entry.path());
      SuffixTree newTree{ifs, idFactory};
      newTree.splitRoot(pre);
      baseTree.merge(newTree);
    }
    baseTree.serialize("./temp_trees/merged_trees/" + pre + ".txt");
    // std::ofstream save;
    // save.open("./temp_trees/merged_trees/" + pre + "_vis.txt");
    // baseTree.visualizeNoLeaves(save);
    // save.close();
    // baseTree.serialize("./temp_trees/merged_trees/" + pre + ".txt");
  }
}

int main() {
  std::string sequence = readSequence("input\\NC_045512v2.fa");
  clearDir("temp_trees");
  clearDir("temp_prfx");
  // // Memory limits experimental section
  // // size_t minWorkingSetSize = 1024 * 1024 * 0.4;
  // // size_t maxWorkingSetSize = 1024 * 1024 * 0.4;
  // // MemoryUtil mu{};
  // // mu.limitMemory(minWorkingSetSize, maxWorkingSetSize);
  // // size_t t = mu.calculate_t(sequence.length(), maxWorkingSetSize);
  
  size_t t = sequence.length()/30;
  // std::cout << t << '\n';
  PrefixCounter pc{static_cast<int>(t), sequence};  
  std::vector<std::string> prefixes = pc.getPrefixes();
  std::cout << prefixes.size() << '\n';
  
  IdFactory idFactory;
  IdFactory* facPointer = &idFactory;
  int partitions = partitionSequence(sequence, t);
  constructTrees("temp_prfx", facPointer, partitions);
  splitTrees(prefixes, facPointer);
  mergeTrees(prefixes, facPointer);
}

// Make list of variable length prefixes
// Partition sequence
// Create implicit suffix tree for each partition
// Make explicit by adding a unique prefix from the start of the next file
// Split trees by prefixes, taking only paths that begin with a given prefix
// Save prefix trees to disk