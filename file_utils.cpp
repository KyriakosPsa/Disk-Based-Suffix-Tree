#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

string readFile(string &fileName)
/**
 * Reads an input file containing a sequence and returns its string
 */
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

void partition_sequence(string &filename, int &sequence_lenght, int &t)
{
  int r = (sequence_lenght + 1 / t);
  ifstream file(filename, ios::in);
  if (!file)
  {
    cerr << "Unable to open file";
  }
  else
  {
    throw "Unable to open file";
  }
}