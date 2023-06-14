#include "file_utils.h"
#include "prefix_count.h"
#include "suffix_tree.h"
#include "partitioning.h"
#include <string>
#include <iostream>
#include <filesystem>

void constructTrees(const string &dir, IdFactory *idFactory, int partitions) {
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
    
    std::string partitionName = entry.path().filename().string();
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

    tree.serialize("./temp_trees/partition_trees/" + partitionName);
    std::cout << partitionName << '\n';
  }
}

void splitTrees(const std::vector<std::string> &prefixes, IdFactory *idFactory) {
  std::ifstream ifs;
  vector<int> location;
  for (auto pre: prefixes) {
    if (!std::filesystem::is_directory("./temp_trees/" + pre) || !std::filesystem::exists("./temp_trees/" + pre)) {
      std::filesystem::create_directory("./temp_trees/" + pre);
    }
  }

  for (const auto& entry : std::filesystem::directory_iterator("./temp_trees/partition_trees/")) {
      ifs.open(entry.path());
      SuffixTree tree{ifs};
      for (auto pre: prefixes) {
        vector<int> location = tree.queryPrefix(pre);
        (*idFactory).getId();
        if (!(location.empty()))
        {
            SuffixTree newTree = *(splitTree(tree, pre, location, idFactory));
            newTree.serialize("./temp_trees/" + pre + '/' + entry.path().filename().string());
        }
        // need prefix location here.
    }
    }
}

int main() {
    std::string sequence = readSequence("./input/small.fasta");
    size_t t = sequence.length()/5;
    std::cout << t << '\n';
    PrefixCounter pc{static_cast<int>(t), sequence};  
    std::vector<std::string> prefixes = pc.getPrefixes();
    std::cout << prefixes.size() << '\n';
    
    IdFactory idFactory;
    IdFactory* facPointer = &idFactory;
    int partitions = partitionSequence(sequence, t);
    constructTrees("temp_prfx", facPointer, partitions);
    splitTrees(prefixes, facPointer);
    // clearDir("temp_trees");
    // clearDir("temp_prfx");
}

// Make list of variable length prefixes
// Partition sequence
// Create implicit suffix tree for each partition
// Make explicit by adding a unique prefix from the start of the next file
// Split trees by prefixes, taking only paths that begin with a given prefix
// Save prefix trees to disk