#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include "partitioning.h"

PartitionUtility::PartitionUtility(int partitions) : m_partitions(partitions)
  {
  }

  void PartitionUtility::setFileName(int fileId) {
    m_fileName = "./temp_prfx/prfx_" + std::to_string(fileId) + ".txt";
  }

  std::string PartitionUtility::getNextPartitionString() {
    setFileName(m_fileId);
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

    setupNextFile();
    return seq;
  }

  // char PartitionUtility::getAdditionalCharacter() {
  //   char c;
  //   m_filestream.get(c);
  //   return c;
  // }

  // open the next file for character-by-character reading,
  // and for getNextPartitionString.
  void PartitionUtility::setupNextFile() {
    m_filestream.close();
    m_fileId++;
    if (m_fileId < m_partitions) {
      setFileName(m_fileId);
      m_filestream.open(m_fileName);
    } else {
      m_finalPartition = true;
    }
  }