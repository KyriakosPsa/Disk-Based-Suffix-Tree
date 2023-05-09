#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <array>
#include <iterator>
#include <vector>
#include <regex>
#include <chrono>
#include <cmath>

using namespace std;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

void readIntoString(string &seq, ifstream &openFile)
{
    auto t1 = high_resolution_clock::now();
    string line;
    getline(openFile, line);
    while (getline(openFile, line))
    {
        if (!line.starts_with('>')) {
            transform(line.begin(), line.end(), line.begin(), ::toupper);
            seq += line;
        }
        
    }

    openFile.clear();
    openFile.seekg(0, std::ios::beg);

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    std::cout << ms_int.count() << "ms\n";
}

unsigned int getFileSize(string fileName)
{
    ifstream is;
    unsigned int length;
    is.open(fileName, ios::binary);
    is.seekg(0, ios::end);
    length = is.tellg();
    return length;
}

int main()
{
    ifstream sequenceFile;
    string file{"GCF_000001405.40_GRCh38.p14_genomic.fna"};
    sequenceFile.open(file);

    unsigned int fileSize = getFileSize(file);

    string sequence;
    sequence.reserve(fileSize);

    cout << fileSize << "\n";

    readIntoString(sequence, sequenceFile);
    cout << sequence.length() << "\n";

    // std::ofstream out("output/output.txt");
    // out << sequence;
    // out.close();
    // sequenceFile.close();
    return 0;
}