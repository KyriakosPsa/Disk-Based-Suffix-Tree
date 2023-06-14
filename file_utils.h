#ifndef FILE_UTILS
#define FILE_UTILS

#include <string>


using namespace std;

// long long int preprocess_sequence(string filename);
string readSequence(string filePath);
void removeFiles(const string &mode = "prefix");
void partitionFile(string &inputFileName, const size_t t);
void clearDir(const string &dir);
int partitionSequence(std::string &inputFileName, const size_t t);

#endif