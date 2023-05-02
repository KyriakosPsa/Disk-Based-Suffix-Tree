#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>
#include <chrono>
using namespace std;

int evalInequality(vector<string> extendSet, size_t currLength, size_t prevLength) {
    size_t sum{0};
    for (size_t i = 1; i <= (currLength - prevLength); i++)
    {
        sum = sum + (4^i);
    }

    return extendSet.size() * sum;
}

void extendPrefixes(vector<string>& extendSet, char (&letterSet)[4]) {
    size_t index{0};

    vector<string> vectorForExtension(extendSet);
    extendSet.resize(extendSet.size()*4);

    for (string prefix: vectorForExtension) {
        for (char letter: letterSet) {
            extendSet[index] = prefix + letter;
            index += 1;
        }
    }
}

void printVector(vector<string>& vector) {
    for (string i: vector) {
        std::cout << i << ' ';
    }
    std::cout << "\n";
}

int countSubstring(string& text, string target) {
    size_t n = text.length();
    size_t target_n = target.length();
    int counter {};
    for (size_t i = 0; i < n; i++) {
        int result = text.compare(i, target_n, target);
        if (result == 0) {
            counter++;
        }
    }
    return counter;
}

void clearEmpties(vector<string>& extendSet) {
    vector<string> newExtendSet;
    for (string prefix: extendSet) {
        if (!prefix.empty()) {
            newExtendSet.push_back(prefix);
        }
    }
    extendSet = newExtendSet;
}

string readFile(string fileName) {
    ifstream myFile(fileName);
    string dna {};
    if (myFile) {
        ostringstream stringStream {};
        stringStream << myFile.rdbuf();
        dna = stringStream.str();
    }
    dna = regex_replace(
        dna, 
        regex("\\r\\n|\\r|\\n"), 
        "");
    myFile.close();
    return dna;
}

int readAndCompare(ifstream& openFile, string target) {
    string line;
    string remainingFragment{""};
    int counter {};
    while (getline(openFile, line)) {
        string compareString = remainingFragment + line;
        transform(compareString.begin(), compareString.end(), compareString.begin(), ::toupper);
        size_t len = compareString.length();
        size_t searchLen = target.length();
        size_t comparableLen = len - searchLen;
        for (size_t i = 0; i < comparableLen; i++) {
            int result = compareString.compare(i, searchLen, target);
            if (result == 0) {
                counter++;
            }
        }
        remainingFragment = compareString.substr(comparableLen);
    }
    return counter;
}

int main() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    ifstream sequenceFile;
    sequenceFile.open("NC_045512v2.fa");
    // string target{"ATACATTCTTATAAAAA"};
    // // sequenceFile.open("./GCF_000001735.4_TAIR10.1_genomic.fna");
    // sequenceFile.open("GCF_000001405.40_GRCh38.p14_genomic.fna");
    // int count = readAndCompare(sequenceFile, "AATCCCT");
    // sequenceFile.seekg(0);
    // sequenceFile.clear();
    // std::cout << count << "\n";
    

    int t{50};
    // string sequence = readFile("NC_045512v2.fa");
    char alphabet[4] {'A', 'G', 'C', 'T'};
    vector<string> extendVector{""};
    vector<string> prefixVector;

    size_t length{0};
    size_t prevLength{0};

    // extendPrefixes(extendVector, alphabet);
    // int index{0};
    // for (string prefix: extendVector) {
    //     int freq = countSubstring(sequence, prefix);
    //     if (freq <= t) {
    //         prefixVector.push_back(prefix);
    //         extendVector[index] = "";
    //     }
    //     index += 1;
    // }
    // std::cout << extendVector.size() << "\n";
    // clearEmpties(extendVector);
    // std::cout << extendVector.size() << "\n";
    // printVector(extendVector);
    // printVector(prefixVector);
    
    while (true) {
        while (evalInequality(extendVector, length, prevLength) <= t) {
            length += 1;
            extendPrefixes(extendVector, alphabet);
            size_t index{0};
            for (string prefix: extendVector) {
                int freq = readAndCompare(sequenceFile, prefix);
                sequenceFile.clear();                 
                sequenceFile.seekg(0, std::ios::beg);
                // std::cout << prefix + " " << freq << "\n";
                if (freq <= t) {
                    prefixVector.push_back(prefix);
                    extendVector[index] = "";
                }
                index += 1;
            }

            clearEmpties(extendVector);
            if (extendVector.size() == 0) {
                break;
            }
        }
        prevLength = length;
        if (extendVector.size() == 0) {
                break;
            }
    }

    std::ofstream output_file("./prefixes.txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    // std::copy(std::begin(prefixVector), std::end(prefixVector), output_iterator);

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
}