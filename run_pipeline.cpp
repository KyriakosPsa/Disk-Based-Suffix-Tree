#include "file_utils.h"
#include "prefix_count.h"
#include "suffix_tree.h"
// #include "partitioning.h"
#include <string>
#include <iostream>
#include <filesystem>

void constructTrees(const string &dir, IdFactory *idFactory) {
  std::string partition;
  if (!std::filesystem::is_directory("temp_trees") || !std::filesystem::exists("temp_trees")) {
    std::filesystem::create_directory("temp_trees");
  }

  if (!std::filesystem::is_directory("./temp_trees/partition_trees") || !std::filesystem::exists("./temp_trees/partition_trees")) {
    std::filesystem::create_directory("./temp_trees/partition_trees");
  }
  
  for (const auto& entry : std::filesystem::directory_iterator(dir)) {
    partition = readSequence(entry.path().string());
    
    std::string partitionName = entry.path().filename().string();
    SuffixTree tree{partition, idFactory};
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
    std::string fileName{"./input/NC_045512v2.fa"};
    std::string sequence = readSequence(fileName);
    int t = sequence.length()/30;

    PrefixCounter pc{t, sequence};  
    std::vector<std::string> prefixes = pc.getPrefixes();
    std::cout << prefixes.size() << '\n';
    
    IdFactory idFactory;
    IdFactory* facPointer = &idFactory;
    partitionFile(fileName, static_cast<size_t>(t));
    constructTrees("temp_prfx", facPointer);
    splitTrees(prefixes, facPointer);
    clearDir("temp_trees/partition_trees");
     // clearDir("temp_prfx");
  
    // ifstream ifs("./temp_trees/AAA/prfx_0.txt");
    // SuffixTree tree2(ifs);
    // ofstream ofs{"./output/visualization.txt"};
    // tree2.visualizeNoLeaves(ofs);

    // string file{"input\\NC_045512v2.fa"};
    // sequenceFile.open(file);

    // unsigned int fileSize = getFileSize(file);
    // string sequence;
    // sequence.reserve(fileSize);
    // cout << fileSize << "\n";
    // readIntoString(sequence, sequenceFile);
    // size_t sequenceLength = sequence.length();
    // std::cout << round(fileSize/3000) << '\n';
    // runSinglePass(sequence, 50, sequenceLength);
    // runMultiPass(sequence, 50, sequenceLength);
    // sequenceFile.close();
    // return 0;
}

// Make list of variable length prefixes
// Partition sequence
// Create implicit suffix tree for each partition
// Make explicit by adding a unique prefix from the start of the next file
// Split trees by prefixes, taking only paths that begin with a given prefix
// Save prefix trees to disk

// Make Node ID global