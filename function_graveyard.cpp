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
