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

int evalInequality(vector<string> extendSet, size_t currLength, size_t prevLength)
{
    size_t sum{0};
    for (size_t i = 1; i <= (currLength - prevLength); i++)
    {
        sum = sum + (4 ^ i);
    }

    return extendSet.size() * sum;
}

void extendPrefixes(vector<string> &extendSet, char (&letterSet)[4])
{
    size_t prefixes{extendSet.size()};

    for (long long unsigned int i = 0; i < prefixes; i++)
    {
        // modify current prefix in place for the first letter.
        // put the rest at the end of extendVector;
        extendSet.push_back(extendSet[i] + letterSet[1]);
        extendSet.push_back(extendSet[i] + letterSet[2]);
        extendSet.push_back(extendSet[i] + letterSet[3]);
        extendSet[i] = extendSet[i] + letterSet[0];
    }
}

void clearEmpties(vector<string> &extendSet)
{
    auto isEmpty = [](const std::string &s) {
        return (s.length() == 0);
    };
    extendSet.erase(std::remove_if(extendSet.begin(), extendSet.end(), isEmpty), extendSet.end());
    // int prefixes{extendSet.size()};
    // for (int i = 0; i < prefixes; i++)
    // {
    //     if (extendSet[i].empty())
    //     {
    //         newExtendSet.push_back(prefix);
    //     }
    // }
    // extendSet = newExtendSet;
}

void printVector(vector<string> &vector)
{
    for (string i : vector)
    {
        std::cout << i << ' ';
    }
    std::cout << "\n";
}

int readAndCompare(ifstream &openFile, string target)
{
    string line;
    string remainingFragment{""};
    int counter{};
    getline(openFile, line);
    while (getline(openFile, line))
    {
        if (!(line.starts_with('>'))) {
            string compareString = remainingFragment + line;
            transform(compareString.begin(), compareString.end(), compareString.begin(), ::toupper);
            size_t len = compareString.length();
            size_t searchLen = target.length();
            size_t comparableLen = len - searchLen;
            for (size_t i = 0; i < comparableLen; i++)
            {
                int result = compareString.compare(i, searchLen, target);
                if (result == 0)
                {
                    counter++;
                }
            }
            remainingFragment = compareString.substr(comparableLen);
        }
    }
    return counter;
}

int readAndCompare(vector<string> &sequenceVector, string target)
{
    string line;
    string remainingFragment{""};
    int counter{};
    for (string line : sequenceVector)
    {
        string compareString = remainingFragment + line;
        size_t len = compareString.length();
        size_t searchLen = target.length();
        size_t comparableLen = len - searchLen;
        for (size_t i = 0; i < comparableLen; i++)
        {
            int result = compareString.compare(i, searchLen, target);
            if (result == 0)
            {
                counter++;
            }
        }
        remainingFragment = compareString.substr(comparableLen);
    }

    return counter;
}

void readAndCompareAll(ifstream &openFile, vector<string> &targets, vector<int> &counts)
{
    string line;
    string remainingFragment{""};

    getline(openFile, line);
    while (getline(openFile, line))
    {
        if (!(line.starts_with('>'))) {
            string compareString = remainingFragment + line;
            transform(compareString.begin(), compareString.end(), compareString.begin(), ::toupper);

            size_t len = compareString.length();
            size_t searchLen = targets[0].length();
            size_t comparableLen = len - searchLen;

            for (size_t i = 0; i < comparableLen; i++)
            {
                size_t index{0};
                for (string target : targets)
                {
                    int result = compareString.compare(i, searchLen, target);
                    if (result == 0)
                    {
                        counts[index] += 1;
                    }
                    index += 1;
                }
            }

            remainingFragment = compareString.substr(comparableLen);
        }
        
    }
}

void readAndCompareAll(vector<string> &sequenceVector, vector<string> &targets, vector<int> &counts)
{
    string line;
    string remainingFragment{""};
    for (string line : sequenceVector)
    {
        string compareString = remainingFragment + line;

        size_t len = compareString.length();
        size_t searchLen = targets[0].length();
        size_t comparableLen = len - searchLen;

        for (size_t i = 0; i < comparableLen; i++)
        {
            size_t index{0};
            for (string target : targets)
            {
                int result = compareString.compare(i, searchLen, target);
                if (result == 0)
                {
                    counts[index] += 1;
                }
                index += 1;
            }
        }

        remainingFragment = compareString.substr(comparableLen);
    }
}

void runMultiPass(ifstream &sequenceFile, int t)
{
    auto t1 = high_resolution_clock::now();

    char alphabet[4]{'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
    extendVector.reserve(10000);
    vector<string> prefixVector;

    size_t length{0};
    size_t prevLength{0};

    while (true)
    {
        while (evalInequality(extendVector, length, prevLength) <= t)
        {
            length += 1;
            extendPrefixes(extendVector, alphabet);
            size_t index{0};
            for (string prefix : extendVector)
            {
                int freq = readAndCompare(sequenceFile, prefix);
                sequenceFile.clear();
                sequenceFile.seekg(0, std::ios::beg);
                // std::cout << prefix + " " << freq << "\n";
                if (freq <= t)
                {
                    prefixVector.push_back(prefix);
                    extendVector[index] = "";
                }
                index += 1;
            }

            clearEmpties(extendVector);
            if (extendVector.size() == 0)
            {
                break;
            }
        }
        prevLength = length;
        if (extendVector.size() == 0)
        {
            break;
        }
    }

    std::ofstream output_file("./output/MultiPassPrefixesStream.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(std::begin(prefixVector), std::end(prefixVector), output_iterator);

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "runMultiPass-sequenceFile: " << ms_double.count() << "ms\n";
}

void runMultiPass(vector<string> &sequenceVector, int t)
{
    auto t1 = high_resolution_clock::now();

    char alphabet[4]{'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
    extendVector.reserve(10000);
    vector<string> prefixVector;

    size_t length{0};
    size_t prevLength{0};

    while (true)
    {
        while (evalInequality(extendVector, length, prevLength) <= t)
        {
            length += 1;
            extendPrefixes(extendVector, alphabet);
            // printVector(extendVector);
            size_t index{0};
            for (string prefix : extendVector)
            {
                int freq = readAndCompare(sequenceVector, prefix);
                // std::cout << prefix + " " << freq << "\n";
                if (freq <= t)
                {
                    prefixVector.push_back(prefix);
                    extendVector[index] = "";
                }
                index += 1;
            }

            clearEmpties(extendVector);
            if (extendVector.size() == 0)
            {
                break;
            }
        }
        prevLength = length;
        if (extendVector.size() == 0)
        {
            break;
        }
    }

    std::ofstream output_file("./output/MultiPassPrefixesVector.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(std::begin(prefixVector), std::end(prefixVector), output_iterator);

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "runMultiPass-sequenceVector: " << ms_double.count() << "ms\n";
}

void runSinglePass(ifstream &sequenceFile, int t)
{
    auto t1 = high_resolution_clock::now();

    char alphabet[4]{'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
    vector<string> prefixVector;
    // vector<int> freqs = vector<int>(16^4);
    // array<int, 5000> freqs{0};
    vector<int> freqs(5000, 0);
    // freqs.reserve(5000);
    // freqs.reserve(65536);

    size_t length{0};
    size_t prevLength{0};

    while (true)
    {
        while (evalInequality(extendVector, length, prevLength) <= t)
        {
            length += 1;
            extendPrefixes(extendVector, alphabet);

            size_t index{0};
            readAndCompareAll(sequenceFile, extendVector, freqs);
            sequenceFile.clear();
            sequenceFile.seekg(0, std::ios::beg);

            for (string prefix : extendVector)
            {
                // std::cout << prefix + " " << freqs[index] << "\n";

                if (freqs[index] <= t)
                {
                    prefixVector.push_back(prefix);
                    extendVector[index] = "";
                }
                freqs[index] = 0;
                index += 1;
            }

            clearEmpties(extendVector);
            if (extendVector.size() == 0)
            {
                break;
            }
        }
        prevLength = length;
        if (extendVector.size() == 0)
        {
            break;
        }
    }

    std::ofstream output_file("./output/SinglePassPrefixesStream.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(std::begin(prefixVector), std::end(prefixVector), output_iterator);

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "runSinglePass-sequenceFile: " << ms_double.count() << "ms\n";
}

void runSinglePass(vector<string> &sequenceVector, int t)
{
    auto t1 = high_resolution_clock::now();

    char alphabet[4]{'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
    vector<string> prefixVector;
    // vector<int> freqs = vector<int>(16^4);
    // array<int, 5000> freqs{0};
    vector<int> freqs(5000, 0);
    // freqs.reserve(5000);

    size_t length{0};
    size_t prevLength{0};

    while (true)
    {
        while (evalInequality(extendVector, length, prevLength) <= t)
        {
            length += 1;
            extendPrefixes(extendVector, alphabet);

            size_t index{0};
            readAndCompareAll(sequenceVector, extendVector, freqs);

            for (string prefix : extendVector)
            {
                // std::cout << prefix + " " << freqs[index] << "\n";

                if (freqs[index] <= t)
                {
                    prefixVector.push_back(prefix);
                    extendVector[index] = "";
                }
                freqs[index] = 0;
                index += 1;
            }

            clearEmpties(extendVector);
            if (extendVector.size() == 0)
            {
                break;
            }
        }
        prevLength = length;
        if (extendVector.size() == 0)
        {
            break;
        }
    }

    std::ofstream output_file("./output/SinglePassPrefixesVector.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(std::begin(prefixVector), std::end(prefixVector), output_iterator);

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "runSinglePass-sequenceVector: " << ms_double.count() << "ms\n";
}

void writeToFile(vector<string> &vec, string fileName)
{
    std::ofstream output_file(fileName);
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(vec.begin(), vec.end(), output_iterator);
}

void readIntoVector(vector<string> &vec, ifstream &openFile, int stringLength)
{
    auto t1 = high_resolution_clock::now();
    string line;
    string grow{""};
    int growCount{0};
    getline(openFile, line);

    while (getline(openFile, line))
    {
        if (!(line.starts_with('>'))) {
            if (growCount == stringLength)
            {
                transform(grow.begin(), grow.end(), grow.begin(), ::toupper);
                vec.emplace_back(grow);
                grow = "";
                growCount = 0;
            }
            grow = grow + line;
            growCount++;
        }
    }

    // to catch final line:
    transform(grow.begin(), grow.end(), grow.begin(), ::toupper);
    vec.emplace_back(grow);
    openFile.clear();
    openFile.seekg(0, std::ios::beg);

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";

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
    // sequenceFile.open("./GCF_000001735.4_TAIR10.1_genomic.fna");
    // sequenceFile.open("GCF_000001405.40_GRCh38.p14_genomic.fna");
    // sequenceFile.open("hello.txt");
    sequenceFile.open("GCF_002966405.1_ASM296640v1_genomic.fna");
    unsigned int fileSize = getFileSize("GCF_002966405.1_ASM296640v1_genomic.fna");
    // unsigned int concatenatedLines = 100;
    unsigned int reserveSize = fileSize / 80 / 100;
    vector<string> sequenceVector;
    sequenceVector.reserve(reserveSize);

    readIntoVector(sequenceVector, sequenceFile, 10);
    printVector(sequenceVector);
    writeToFile(sequenceVector, "test.txt");

    int t = 10;// round(fileSize / 3000);
    cout << "threshold t: " << t << "\n";
    // runSinglePass(sequenceFile, t);
    // runSinglePass(sequenceVector, t);
    // runMultiPass(sequenceFile, t);
    runMultiPass(sequenceVector, t);
}