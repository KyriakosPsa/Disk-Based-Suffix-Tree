#ifndef PARTITIONING
#define PARTITIONING

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

class PartitionUtility {
    private:
        // int m_fileId{0};
        // int m_partitions{0};
        // std::ifstream m_filestream;
        std::string m_fileName = "";
        // std::streampos m_position{0};
        void setFileName(std::string &entry);
        // void setupNextFile(std::string &entry);

    public:
        // PartitionUtility(int partitions);
        std::string getPartitionString(std::string &entry);
        // // char getAdditionalCharacter();
        // bool m_finalPartition{false};
};


#endif