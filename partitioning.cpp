#include <iostream>
#include <fstream>
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
      string tmpfilename = "prfx_" + to_string(filecount) + ".txt";
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

int main()
{
  // Test the ReadFile function
  string inputFileName = "input.txt";
  const int t = 100;
  int position = 0;
  ReadWritefile(inputFileName, t, position);
  return 0;
}