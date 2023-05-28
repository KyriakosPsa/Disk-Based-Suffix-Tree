#include <iostream>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <thread>

using namespace std;

void ReadWritefile(const string &inputFileName, const int t, int position = 0)
{
  int fileposition = position;       // The position of the pointer in the file
  ifstream inputFile(inputFileName); // Open input file stream
  if (!inputFile)
  {
    cerr << "Failed to open input file.";
    return;
  }
  char buffer[t]; // Define a region in memory to contain the t characters to read
  int filecount = 0;
  while (!inputFile.eof())
  {
    inputFile.seekg(fileposition);      // Move the pointer to that position
    inputFile.read(buffer, t);          // Read characters from input file
    int bytesRead = inputFile.gcount(); // Get the number of characters read
    if (bytesRead > 0)
    {
      const string tmpfilename = "prfx_" + to_string(filecount) + ".txt";
      ofstream outputFile(tmpfilename); // Open output file stream in "tmp" folder
      if (!outputFile)
      {
        cerr << "Failed to open output file.";
        return;
      }
      outputFile.write(buffer, bytesRead); // Write characters to output file
      outputFile.close();
      fileposition += t;
      filecount += 1;
    }
    else
    {
      // Reached the end of the file
      inputFile.close(); // Close input file stream
      break;
    }
  }
}

void removeFiles(const string &mode = "prefix")
{
  int status{0};        // remove returns 0 if a file is removed
  int filecount{0};     // the id tag used in our naming shceme
  if (mode == "prefix") // prefix mode, delete prefix files
  {
    const string fileprefix = "prfx_";
    const string filesuffix = ".txt";
    while (status == 0)
    {
      string filename = fileprefix + to_string(filecount) + filesuffix;
      status = remove(filename.c_str());
      filecount++;
    }
    return;
  }
  else if (mode == "suffix_tree") // suffix tree mode, delete suffix tree files
  {
    while (status == 0)
    {
      // Awaiting implementation
    }
    return;
  }
  else
  {
    cerr << "Invalid mode";
    return;
  }
}

int main()
{
  // Test the ReadFile function
  string inputFileName = "input.txt";
  const int t = 2;
  int position = 0;
  ReadWritefile(inputFileName, t, position);
  string del_mode = "prefix";
  removeFiles(del_mode);
  return 0;
}
