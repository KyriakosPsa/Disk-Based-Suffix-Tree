#ifndef FILE_UTILS
#define FILE_UTILS

#include <string>

// long long int preprocess_sequence(string filename);
std::string readSequence(std::string filePath);
void removeFiles(const std::string &mode = "prefix");
void partitionFile(std::string &inputFileName, const size_t t);
void clearDir(const std::string &dir);
int partitionSequence(std::string &inputFileName, const size_t t);

#endif