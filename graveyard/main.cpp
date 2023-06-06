#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <cmath>
#include "header.h"

using namespace std;

int main()
{
  // ask for the filename
  string file_input_text;
  long long int input_sequence_len = 0;
  cout << "Please pass the name of the file containing the DNA sequence:\n";
  cin >> file_input_text;
  // Read the file and find the lenght of the sequence
  try
  {
    input_sequence_len = preprocess_sequence(file_input_text);
  }
  catch (const exception &e)
  {
    cerr << e.what() << '\n';
    exit(1); // Stop execution
  }
  cout << "Sequence length is: " << input_sequence_len << "\n";

  // Initialize variables for the calculation of t
  long long int limited_ram_bytes = 0;
  int limited_memory_GB;
  char answer;
  bool limit_memory_bool;

  // Ask the user if they want to limit the memory of the Algorithm in GB
  cout << "Do you wish to limit the available memory for the algorithm? (y/n): \n";
  cin >> answer;
  if (answer == 'y')
  {
    limit_memory_bool = true;
    cout << "Enter the RAM memory you wish to use (in GBs):\n";
    cin >> limited_memory_GB;
    limited_ram_bytes = limited_memory_GB * pow(10, 9);
  }
  else
  {
    limit_memory_bool = false;
  }

  // Get the total available memory
  long long int available_total_RAM = get_total_ram_bytes();

  // Calculate t, throw error if limit_memory > available RAM
  try
  {
    long long int t = calculate_t(input_sequence_len,
                                  available_total_RAM,
                                  limited_ram_bytes,
                                  limit_memory_bool);
    cout << "The value of t is:\n"
         << t;
  }
  catch (const exception &e)
  {
    cerr << e.what() << '\n';
    exit(1); // Stop execution
  }
}