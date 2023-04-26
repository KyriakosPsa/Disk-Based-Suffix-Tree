#include <regex>
#include <iostream>
#include <string>
#include <vector>
#include "header.h"

using namespace std;

int main()
{
  // Calculate t test
  int seq_size = 0;
  int limit_memory_to = 0;
  // Pass sequence lenght
  cout << "Enter your sequence lenght:\n";
  cin >> seq_size;
  // pass available RAM
  cout << "Enter the RAM memory you wish to use:\n";
  cin >> limit_memory_to;
  try
  {
    int t = calculate_t(seq_size, true, limit_memory_to);
    cout << "The value of t is:\n"
         << t;
  }
  catch (std::exception &e)
  { // catch the exception
    cerr << "Error: " << e.what();
  }
  // calculate partitions test
}