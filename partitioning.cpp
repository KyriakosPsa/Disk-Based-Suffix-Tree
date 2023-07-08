#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include "partitioning.h"

// PartitionUtility::PartitionUtility(int partitions) : m_partitions(partitions)
//   {
//   }

  void PartitionUtility::setFileName(std::string &entry) {
    m_fileName = "./temp_prfx/" + entry;
  }

  std::string PartitionUtility::getPartitionString(std::string &entry) {
    setFileName(entry);
    std::ifstream ifs;
    std::string line;
    std::string seq;
    ifs.open(m_fileName);
    while (std::getline(ifs, line))
    {
        if (!line.starts_with('>')) {
            seq += line;
        }
        
    }
    ifs.close();
    return seq;
  }

  // char PartitionUtility::getAdditionalCharacter() {
  //   char c;
  //   m_filestream.get(c);
  //   return c;
  // }

  // open the next file for character-by-character reading,
  // and for getNextPartitionString.
  // void PartitionUtility::setupNextFile() {
  //   m_filestream.close();
  //   m_fileId++;
  //   if (m_fileId < m_partitions) {
  //     setFileName(m_fileId);
  //     m_filestream.open(m_fileName);
  //   } else {
  //     m_finalPartition = true;
  //   }
  // }