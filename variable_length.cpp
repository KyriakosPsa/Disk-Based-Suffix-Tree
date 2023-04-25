#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>
using namespace std;

int evalInequality(vector<string> extendSet, int currLength, int prevLength) {
    int sum{0};
    for (size_t i = 1; i <= (currLength - prevLength); i++)
    {
        sum = sum + 4^i;
    }

    return extendSet.size() * sum;
}

void extendPrefixes(vector<string>& extendSet, char (&letterSet)[4]) {
    int index{0};

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

int main() {
    string sequence = readFile("hello.txt");
    int t{50};

    char alphabet[4] {'a', 'g', 'c', 't'};
    vector<string> extendVector{""};
    vector<string> prefixVector;

    int length{0};
    int prevLength{0};

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
            int index{0};
            for (string prefix: extendVector) {
                int freq = countSubstring(sequence, prefix);
                std::cout << prefix + " " << freq << "\n";
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
    std::copy(std::begin(prefixVector), std::end(prefixVector), output_iterator);
}