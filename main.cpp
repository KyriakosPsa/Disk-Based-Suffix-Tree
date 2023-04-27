#include <regex>
#include <iostream>
#include <string>
#include <vector>
#include "header.h"

using namespace std;

int main()
{
  // Initialize variables for the calculation of t
  long long int seq_size = 3200000000;
  int limit_memory_to;
  char answer;
  bool limit_memory;

  // Ask the user if they want to limit the memory of the Algorithm in GB
  cout << "Do you wish to limit the available memory of the algorithm? (y/n):";
  cin >> answer;
  if (answer == 'y')
  {
    limit_memory = true;
    cout << "Enter the RAM memory you wish to use:\n";
    cin >> limit_memory_to;
  }
  else
  {
    limit_memory = false;
  }

  // Calculate t
  try
  {
    long long int t = calculate_t(seq_size, limit_memory, limit_memory_to);
    cout << "The value of t is:\n"
         << t;
  }
  catch (std::exception &e)
  { // catch the exception if limited memory > RAM
    cerr << "Error: " << e.what();
  }
}