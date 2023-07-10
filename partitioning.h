#ifndef PARTITIONING
#define PARTITIONING

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

class PartitionUtility {
    private:
        std::string m_fileName = "";
        void setFileName(std::string &entry);

    public:
        std::string getPartitionString(std::string &entry);
};


#endif