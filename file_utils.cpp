#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

long long int preprocess_sequence(string filename)
/**
 * Removes the endline characters, counts the total string lenght
 * and writes the one line total string in a new file
 * @param filename the input file to read line by line
 *  */
{
  // open the file to read from and write to
  ifstream input_file(filename);
  ofstream output_file("oneline_" + filename);
  if (input_file.is_open() && output_file.is_open())
  {
    string line;
    long long int char_count = 0;
    // Get each of the lines
    while (getline(input_file, line))
    {
      if (!line.empty() && line.back() == '\n')
      {
        // Remove "\n"
        line.pop_back();
      }
      // count the characters of the line without "\n"
      char_count += line.length();
      // Write the one-line string to a new file
      output_file << line;
    }
    // Return the total results for all the string
    cout << "Total characters processed: " << char_count << "\n";
    input_file.close();
    output_file.close();
    return char_count;
  }
  else
  {
    cerr << "Error: could not open files";
    exit(1);
  }
}