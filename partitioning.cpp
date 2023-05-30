#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

using namespace std;

class fileOp
{
public:
  string m_inputFileName = "";
  string m_filesuffix;
  string m_fileprefix;
  int m_fileid{0};
  int m_filepos{0};
  fstream m_nextinputFile;
  fileOp()
  {
  }
  // input file name is required for all the methods except the removeFiles
  fileOp(string inputFileName) : m_inputFileName(inputFileName)
  {
    // identify suffix, prefix of file and fileid
    size_t underscorePos = m_inputFileName.find('_');
    size_t dotPos = m_inputFileName.find('.');
    string fileIDString = m_inputFileName.substr(underscorePos + 1, dotPos - underscorePos - 1);
    m_fileid = stoi(fileIDString); // string to int
    m_fileprefix = m_inputFileName.substr(0, dotPos - 1);
    m_filesuffix = m_inputFileName.substr(dotPos);

    // construct the filename of the next file and open its filestream
    string nextfilename = m_fileprefix + to_string(m_fileid + 1) + m_filesuffix;
    m_nextinputFile.open(nextfilename); // open the stream of the next file
    if (!m_nextinputFile)
    {
      cerr << "Failed to open input file.";
    }
  }

  char searchNextunique()
  // Open the input file and get the character at the specified position default = 0
  {
    char c;
    m_nextinputFile.seekg(m_filepos);
    c = m_nextinputFile.get();
    m_filepos++; // When called again it moves to the next character and returns it
    return c;
  };

  void releaseFile()
  // close the class member next file stream
  {
    m_nextinputFile.close();
    return;
  }
};
/// End of class
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

void partitionFile(string &inputFileName, const size_t t)
// Open input file and exhaustively partition it in t files : prfx_1.txt, to prfx_2.txt etc.
{
  int fileposition{0};               // The position of the pointer in the file
  ifstream inputFile(inputFileName); // Open input file stream
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

int main()
{
  // Test the ReadWritefile method
  const size_t t = 100;
  string file = "./input.txt";
  partitionFile(file, t);
  // Test the ReadWritefile method
  fileOp myFileOp{"prfx_0.txt"};
  char testchar;
  for (int a = 0; a <= 80; a++)
  {
    testchar = myFileOp.searchNextunique();
    cout << testchar << "\n";
    if (a == 80)
    {
      myFileOp.releaseFile();
      removeFiles();
    }
  }
  return 0;
}
