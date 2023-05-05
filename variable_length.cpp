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

using namespace std;

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
    size_t index{0};

    vector<string> vectorForExtension(extendSet);
    extendSet.resize(extendSet.size() * 4);

    for (string prefix : vectorForExtension)
    {
        for (char letter : letterSet)
        {
            extendSet[index] = prefix + letter;
            index += 1;
        }
    }
}

void printVector(vector<string> &vector)
{
    for (string i : vector)
    {
        std::cout << i << ' ';
    }
    std::cout << "\n";
}

int countSubstring(string &text, string target)
{
    size_t n = text.length();
    size_t target_n = target.length();
    int counter{};
    for (size_t i = 0; i < n; i++)
    {
        int result = text.compare(i, target_n, target);
        if (result == 0)
        {
            counter++;
        }
    }
    return counter;
}

void clearEmpties(vector<string> &extendSet)
{
    vector<string> newExtendSet;
    for (string prefix : extendSet)
    {
        if (!prefix.empty())
        {
            newExtendSet.push_back(prefix);
        }
    }
    extendSet = newExtendSet;
}

int readAndCompare(ifstream &openFile, string target)
{
    string line;
    string remainingFragment{""};
    int counter{};
    while (getline(openFile, line))
    {
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

void readAndCompareAll(ifstream &openFile, vector<string> &targets, array<int, 5000> &counts)
{
    string line;
    string remainingFragment{""};

    getline(openFile, line);
    while (getline(openFile, line))
    {
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

void readAndCompareAll(vector<string> &sequenceVector, vector<string> &targets, array<int, 5000> &counts)
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
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    char alphabet[4]{'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
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
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    char alphabet[4]{'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
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
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    char alphabet[4]{'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
    vector<string> prefixVector;
    // vector<int> freqs = vector<int>(16^4);
    array<int, 5000> freqs{0};
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
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    char alphabet[4]{'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
    vector<string> prefixVector;
    // vector<int> freqs = vector<int>(16^4);
    array<int, 5000> freqs{0};
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
    string line;
    string grow{""};
    int growCount{0};
    getline(openFile, line);
    long int progrescounter = 0;
    while (getline(openFile, line))
    {
        if (growCount == stringLength)
        {
            transform(grow.begin(), grow.end(), grow.begin(), ::toupper);
            vec.emplace_back(grow);
            grow = "";
            growCount = 0;
        }
        grow = grow + line;
        growCount++;
        progrescounter++;
        if ((progrescounter % 40000) == 0)
        {
            cout << progrescounter << "\n";
        }
    }

    // to catch final line:
    vec.emplace_back(grow);
    openFile.clear();
    openFile.seekg(0, std::ios::beg);
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
    sequenceFile.open("genome.fna");
    unsigned int file_size = getFileSize("genome.fna");
    // unsigned int concatenatedLines = 100;
    unsigned int reserveSize = file_size / 80 / 100;
    vector<string> sequenceVector;
    sequenceVector.reserve(reserveSize);

    readIntoVector(sequenceVector, sequenceFile, 99);
    // writeToFile(sequenceVector, "test.txt");

    // unsigned int t = round(file_size / 3000);
    // cout << "File size: " << file_size << "\n";
    // cout << "threshold t: " << t << "\n";
    // runSinglePass(sequenceFile, t);
    // runSinglePass(sequenceVector, t);
    // runMultiPass(sequenceFile, t);
    // runMultiPass(sequenceVector, t);
}
