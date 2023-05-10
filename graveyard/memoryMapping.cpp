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
#include <iostream>
#include <Windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int evalInequality(std::vector<std::string> extendSet, size_t currLength, size_t prevLength)
{
    size_t sum{ 0 };
    for (size_t i = 1; i <= (currLength - prevLength); i++)
    {
        sum = sum + (4 ^ i);
    }

    return extendSet.size() * sum;
}

void extendPrefixes(std::vector<std::string>& extendSet, char(&letterSet)[4])
{
    size_t prefixes{ extendSet.size() };

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

void clearEmpties(std::vector<std::string>& extendSet)
{
    auto isEmpty = [](const std::string& s) {
        return (s.length() == 0);
    };
    extendSet.erase(std::remove_if(extendSet.begin(), extendSet.end(), isEmpty), extendSet.end());
}

void printVector(std::vector<std::string>& vector)
{
    for (std::string i : vector)
    {
        std::cout << i << ' ';
    }
    std::cout << "\n";
}

void printVector(std::vector<int>& vector)
{
    for (int i : vector)
    {
        std::cout << i << ' ';
    }
    std::cout << "\n";
}

void writeToFile(std::vector<std::string>& vec, std::string fileName)
{
    std::ofstream output_file(fileName);
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(vec.begin(), vec.end(), output_iterator);
}

int readAndCompare(char* sequence, std::string target, int t)
{
    int counter{ 0 };
    size_t searchLen = target.length();
    std::string subStr{""};
    
    for (size_t pos{ 0 }; sequence[pos] != '\0'; pos++) {
        subStr.assign(&sequence[pos], &sequence[pos + searchLen]);
        int result = subStr.compare(target);
            if (result == 0) {
                counter+= 1;
                if (counter == t) {
                    return counter;
                }
            }
    }
    return counter;
}

void readAndCompareAll(char* sequence, std::vector<std::string>& targets, std::vector<std::string>& goNext, std::vector<int>& counts, int t)
{    
    size_t searchLen = targets[0].length();
    std::string subStr;
    for (size_t pos{ 0 }; sequence[pos] != '\0'; pos++) {
        subStr.assign(&sequence[pos], &sequence[pos + searchLen]);
        size_t index{ 0 };
            for (std::string target : targets)
            {
                if (!target.empty()) {
                    int result = subStr.compare(target);
                    if (result == 0)
                    {
                        counts[index] += 1;
                        if (counts[index] >= t) {
                            goNext.emplace_back(targets[index]);
                            targets[index] = "";
                            counts[index] = 0;
                        }
                    }
                }
                index += 1;
            }
    }
}

void runMultiPass(char* sequence, int t)
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
                int freq = readAndCompare(sequence, prefix, t);
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

void runSinglePass(char* sequence, int t)
{
    auto t1 = high_resolution_clock::now();

    char alphabet[4]{ 'A', 'G', 'C', 'T' };
    std::vector<std::string> extendVector;
    std::vector<std::string> goNextVector{ "" };
    std::vector<std::string> prefixVector;
    std::vector<int> freqs(10000, 0);

    size_t length{ 0 };
    size_t prevlength{ 0 };

    while (true)
    {
        while (evalInequality(extendVector, length, prevlength) <= t)
        {
            extendVector = goNextVector;
            goNextVector.clear();
            length += 1;
            extendPrefixes(extendVector, alphabet);

            readAndCompareAll(sequence, extendVector, goNextVector, freqs, t);

            clearEmpties(extendVector);
            prefixVector.insert(std::end(prefixVector), std::begin(extendVector), std::end(extendVector));
            std::fill(freqs.begin(), freqs.end(), 0);
            if (goNextVector.size() == 0)
            {
                break;
            }
        }
        prevlength = length;
        if (goNextVector.size() == 0)
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

void makeOneLine(std::string fileName) {
    std::ifstream t(fileName);
    std::string str{ "" };
    std::string line;
    std::getline(t, line);
    while(std::getline(t, line)) {
        if (!line.starts_with('>')) {
            str = str + line;
        }
    }
    transform(str.begin(), str.end(), str.begin(), ::toupper);

    std::ofstream out("output/output.txt");
    out << str;
    out.close();
}

unsigned int getFileSize(std::string fileName)
{
    std::ifstream is;
    unsigned int length;
    is.open(fileName, std::ios::binary);
    is.seekg(0, std::ios::end);
    length = is.tellg();
    return length;
}



int main()
{
    makeOneLine("NC_045512v2.fa");

    std::string fileName = "./output/output.txt";
    std::wstring stemp = std::wstring(fileName.begin(), fileName.end());
    LPCWSTR sw = stemp.c_str();
    HANDLE fileHandle = CreateFileW(sw, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        std::cerr << fileHandle << std::endl;
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    // Get the file size
    DWORD fileSizeHigh;
    DWORD fileSizeLow = GetFileSize(fileHandle, &fileSizeHigh);
    if (fileSizeLow == INVALID_FILE_SIZE && GetLastError() != NO_ERROR) {
        std::cerr << "Failed to determine the file size." << std::endl;
        CloseHandle(fileHandle);
        return 1;
    }

    // Calculate the total file size
    ULARGE_INTEGER fileSize;
    fileSize.LowPart = fileSizeLow;
    fileSize.HighPart = fileSizeHigh;

    // Create a file mapping object
    HANDLE mappingHandle = CreateFileMapping(fileHandle, nullptr, PAGE_READONLY, fileSize.HighPart, fileSize.LowPart, nullptr);
    if (mappingHandle == nullptr) {
        std::cerr << "Failed to create file mapping." << std::endl;
        CloseHandle(fileHandle);
        return 1;
    }

    // Map the file to memory
    LPVOID mappedData = MapViewOfFile(mappingHandle, FILE_MAP_READ, 0, 0, 0);
    if (mappedData == nullptr) {
        std::cerr << "Failed to map the file into memory." << std::endl;
        CloseHandle(mappingHandle);
        CloseHandle(fileHandle);
        return 1;
    } 

    // Access the mapped file
    char* fileContent = static_cast<char*>(mappedData);
    // std::string fileString(fileContent);
    // std::cout << "File content: " << fileString << std::endl;
    unsigned int fileSizeInt = getFileSize(fileName);
    int t = round(fileSizeInt / 3000);
    std::cout << "t: " << t << '\n';
    runSinglePass(fileContent, t);
    runMultiPass(fileContent, t);

    // Clean up
    UnmapViewOfFile(mappedData);
    CloseHandle(mappingHandle);
    CloseHandle(fileHandle);

    return 0;
}