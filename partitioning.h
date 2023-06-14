#ifndef PARTITIONING
#define PARTITIONING

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;

class PartitionUtility {
    private:
        int m_fileId{0};
        int m_partitions{0};
        ifstream m_filestream;
        string m_fileName = "";
        streampos m_position{0};
        void setFileName(int fileId);
        void setupNextFile();

    public:
        PartitionUtility(int partitions);
        std::string getNextPartitionString();
        // char getAdditionalCharacter();
        bool m_finalPartition{false};
};


#endif