#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class fileOp
{
public:
  int m_fileId{0};
  fstream m_filestream;
  string m_fileName = "";
  streampos m_position{0};

  fileOp(int fileId, int position) : m_fileId(fileId), m_position(position)
  {
    openFileStream();
  }

  char readCharacter()
  {
    char c;
    m_filestream.seekg(m_position); // If we want to start from a position in the file
    m_filestream.get(c);
    if (m_filestream.eof())
    {
      m_position = 0;
      releaseFileStream();
      nextFile();
      openFileStream();
      m_filestream.get(c);
    }
    m_position = m_filestream.tellg(); // Update the current position
    return c;
  }

private:
  void openFileStream()
  {
    m_fileName = "prfx_" + to_string(m_fileId) + ".txt";
    m_filestream.open(m_fileName);
    if (!m_filestream)
    {
      cerr << "Failed to open input file." << endl;
    }
  }

  void releaseFileStream()
  {
    m_filestream.close();
  }

  void nextFile()
  {
    m_fileId++;
    m_fileName = "prfx_" + to_string(m_fileId) + ".txt";
  }
};

// // Testing
// int main()
// {
//   fileOp myFileOp{0, 0};
//   char testChar;
//   int t = 0;
//   while (true)
//   {
//     testChar = myFileOp.readCharacter();
//     cout << testChar << "\n";
//     t++;
//     if (t == 50)
//     {
//       break;
//     }
//   }
//   return 0;
// }