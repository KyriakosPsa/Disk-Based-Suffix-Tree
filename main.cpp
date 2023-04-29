#include <regex>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "header.h"

using namespace std;

int main()
{
  // find the sequence_size from the file
  // string filename;
  // cout << "Enter the DNA sequence filename: \n";
  // cin >> filename;
  long long int seq_size = 3200000000;

  // Initialize variables for the calculation of t
  long long int limited_ram_bytes;
  int limited_memory_GB;
  char answer;
  bool limit_memory_bool;

  // Ask the user if they want to limit the memory of the Algorithm in GB
  cout << "Do you wish to limit the available memory for the algorithm? (y/n): \n";
  cin >> answer;
  if (answer == 'y')
  {
    limit_memory_bool = true;
    cout << "Enter the RAM memory you wish to use:\n";
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
    if (limited_ram_bytes < available_total_RAM)
    {
      long long int t = calculate_t(seq_size,
                                    available_total_RAM,
                                    limit_memory_bool,
                                    limited_ram_bytes);
      cout << "The value of t is:\n"
           << t;
    }
    else
    {
      throw invalid_argument("Limited memory cannot be higher than available system RAM");
    }
  }
  catch (invalid_argument &e)
  {
    cerr << e.what() << "\n";
  }
}