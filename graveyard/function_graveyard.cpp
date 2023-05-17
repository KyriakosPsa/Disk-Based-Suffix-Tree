#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;

string readFile(string fileName)
{
    ifstream myFile(fileName);
    string dna{};
    if (myFile)
    {
        ostringstream stringStream{};
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