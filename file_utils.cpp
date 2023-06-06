#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <cmath>
#include "file_utils.h"
#include <filesystem>

using namespace std;

// long long int preprocess_sequence(string filename)
// /**
//  * Removes the endline characters, counts the total string lenght
//  * and writes the one line total string in a new file
//  * @param filename the input file to read line by line
//  *  */
// {
//   // open the file to read from and write to
//   ifstream input_file(filename);
//   ofstream output_file("oneline_" + filename);
//   if (input_file.is_open() && output_file.is_open())
//   {
//     string line;
//     long long int char_count = 0;
//     // Get each of the lines
//     while (getline(input_file, line))
//     {
//       if (!line.empty() && line.back() == '\n')
//       {
//         // Remove "\n"
//         line.pop_back();
//       }
//       // count the characters of the line without "\n"
//       char_count += line.length();
//       // Write the one-line string to a new file
//       output_file << line;
//     }
//     // Return the total results for all the string
//     cout << "Total characters processed: " << char_count << "\n";
//     input_file.close();
//     output_file.close();
//     return char_count;
//   }
//   else
//   {
//     cerr << "Error: could not open files";
//     exit(1);
//   }
// }

string readSequence(string filePath) {
    string line;
    string seq;
    ifstream ifs{filePath};
    getline(ifs, line);
    while (getline(ifs, line))
    {
        if (!line.starts_with('>')) {
            transform(line.begin(), line.end(), line.begin(), ::toupper);
            seq += line;
        }
        
    }
    ifs.close();
    return seq;
}

void removeFiles(const string &mode)
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

void clearDir(const string &dir) {
  for (const auto& entry : std::filesystem::directory_iterator(dir)) 
        std::filesystem::remove_all(entry.path());
  std::filesystem::remove(dir);
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
  if (!std::filesystem::is_directory("temp_prfx") || !std::filesystem::exists("temp_prfx")) {
    std::filesystem::create_directory("temp_prfx");
  }
  while (!inputFile.eof())
  {
    inputFile.seekg(fileposition);         // Move the pointer to that position
    inputFile.read(buffer.data(), (int)t); // Read characters from input file
    int bytesRead = inputFile.gcount();    // Get the number of characters read
    if (bytesRead > 0)
    {
      const string tmpfilename = "./temp_prfx/prfx_" + to_string(filecount) + ".txt";
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