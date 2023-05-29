#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

using namespace std;

class fileOp
{
public:
  string m_inputFileName = "";
  fileOp()
  {
  }
  // input file name is required for all the methods except the removeFiles
  fileOp(string inputFileName) : m_inputFileName(inputFileName)
  {
  }

  char searchNextunique(int position = 0)
  // Open the input file and get the character at the specified position
  {
    ifstream inputFile(m_inputFileName);
    if (!inputFile)
    {
      cerr << "Failed to open input file.";
    }
    char c;
    inputFile.seekg(position);
    c = inputFile.get();
    return c;
    inputFile.close();
  };

  void ReadWritefile(const size_t t, int position = 0)
  // Open input file and exhaustively partition it in t files : prfx_1.txt, to prfx_2.txt etc.
  {
    int fileposition = position;         // The position of the pointer in the file
    ifstream inputFile(m_inputFileName); // Open input file stream
    if (!inputFile)
    {
      cerr << "Failed to open input file.";
      return;
    }
    vector<char> buffer(t); // Define a region in memory to contain the t characters to read
    int filecount = 0;
    while (!inputFile.eof())
    {
      inputFile.seekg(fileposition);         // Move the pointer to that position
      inputFile.read(buffer.data(), (int)t); // Read characters from input file
      int bytesRead = inputFile.gcount();    // Get the number of characters read
      if (bytesRead > 0)
      {
        const string tmpfilename = "prfx_" + to_string(filecount) + ".txt";
        ofstream outputFile(tmpfilename); // Open output file stream in "tmp" folder
        if (!outputFile)
        {
          cerr << "Failed to open output file.";
          return;
        }
        outputFile.write(buffer.data(), bytesRead); // Write characters to output file
        outputFile.close();
        fileposition += (int)t;
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
  // delete files based on mode, either all the prefix files or suffix tree files
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
};

int main()
{
  // Test the ReadWritefile method
  const size_t t = 100;
  fileOp myFileOp1("input.txt");
  myFileOp1.ReadWritefile(t);
  // Test the ReadWritefile method
  fileOp myFileOp2("prfx_0.txt");
  char testchar;
  testchar = myFileOp2.searchNextunique();
  cout << testchar;
  // Test the removeFiles method
  fileOp myFileOp3{};
  myFileOp3.removeFiles();
  return 0;
}
