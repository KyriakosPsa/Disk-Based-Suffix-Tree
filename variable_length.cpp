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
}

void writeToFile(std::vector<std::string>& vec, std::string fileName)
{
    std::ofstream output_file(fileName);
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(vec.begin(), vec.end(), output_iterator);
}

int countSubstring(string &sequence, std::string target, int t, size_t sequenceLength)
{
    int counter{ 0 };
    size_t searchLen = target.length();
    
    for (size_t i = 0; i < sequenceLength; i++) {
        int result = sequence.compare(i, searchLen, target);
            if (result == 0) {
                counter+= 1;
                if (counter == t) {
                    return counter;
                }
            }
    }
    return counter;
}

void runMultiPass(string &sequence, int t, size_t sequenceLength)
{
    auto t1 = high_resolution_clock::now();

    char alphabet[4]{ 'A', 'G', 'C', 'T' };
    std::vector<std::string> extendVector{ "" };
    extendVector.reserve(10000);
    std::vector<std::string> prefixVector;

    size_t length{ 0 };
    size_t prevlength{ 0 };

    while (true)
    {
        while (evalInequality(extendVector, length, prevlength) <= t)
        {
            length += 1;
            extendPrefixes(extendVector, alphabet);
            size_t index{ 0 };
            for (std::string prefix : extendVector)
            {
                int freq = countSubstring(sequence, prefix, t, sequenceLength);
                if (freq < t)
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
        prevlength = length;
        if (extendVector.size() == 0)
        {
            break;
        }
    }
    writeToFile(prefixVector, "output/multiOutput.txt");
    //std::ofstream output_file("./output/multipassprefixesvector.txt");
    //std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    //std::copy(std::begin(prefixvector), std::end(prefixvector), output_iterator);

    auto t2 = high_resolution_clock::now();

    /* getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "MultiPass: " << ms_double.count() << "ms\n";
}

void countAllSubstrings(string &sequence, std::vector<std::string>& targets, std::vector<int>& counts, int t, size_t sequenceLength)
{    
    size_t searchLen = targets[0].length();
    size_t done{0};
    for (size_t i = 0; i < sequenceLength; i++) {
        size_t index{ 0 };
        for (std::string target : targets)
        {
            if (counts[index] < t) {
                int result = sequence.compare(i, searchLen, target);
                if (result == 0)
                {
                    counts[index] += 1;
                }
                if (counts[index] == t) {
                    done++;
                }
            }
            if (done == targets.size()) {
                break;
            }
            index += 1;
        }
        if (done == targets.size()) {
                break;
            }
    }
}

void runSinglePass(string &sequence, int t, size_t sequenceLength)
{
    auto t1 = high_resolution_clock::now();

    char alphabet[4]{ 'A', 'G', 'C', 'T' };
    std::vector<std::string> extendVector{""};
    extendVector.reserve(10000);
    std::vector<std::string> prefixVector;
    std::vector<int> freqs(10000, 0);

    size_t length{ 0 };
    size_t prevlength{ 0 };

    while (true)
    {
        while (evalInequality(extendVector, length, prevlength) <= t)
        {
            length += 1;
            extendPrefixes(extendVector, alphabet);

            countAllSubstrings(sequence, extendVector, freqs, t, sequenceLength);

            size_t index{0};
            for(string prefix: extendVector) {
                if (freqs[index] < t)
                {
                    prefixVector.push_back(prefix);
                    extendVector[index] = "";
                }
                index += 1;
            }

            clearEmpties(extendVector);
            prefixVector.insert(std::end(prefixVector), std::begin(extendVector), std::end(extendVector));
            std::fill(freqs.begin(), freqs.end(), 0);
            if (extendVector.size() == 0)
            {
                break;
            }
        }
        prevlength = length;
        if (extendVector.size() == 0)
        {
            break;
        }
    }
    writeToFile(prefixVector, "output/singleOutput.txt");
    auto t2 = high_resolution_clock::now();

    /* getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "Single Pass: " << ms_double.count() << "ms\n";
}

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
    string file{"input\\NC_045512v2.fa"};
    sequenceFile.open(file);

    unsigned int fileSize = getFileSize(file);
    string sequence;
    sequence.reserve(fileSize);
    cout << fileSize << "\n";
    readIntoString(sequence, sequenceFile);
    size_t sequenceLength = sequence.length();
    std::cout << round(fileSize/3000) << '\n';
    runSinglePass(sequence, round(fileSize/3000), sequenceLength);
    runMultiPass(sequence, round(fileSize/3000), sequenceLength);
    sequenceFile.close();
    return 0;
}