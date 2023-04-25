#include <regex>
#include <iostream>
#include <string>
#include "header.h"

using namespace std;

int main()
{
  // Calculate t
  int seq_size = 1000000;
  float t = calculate_t(seq_size);
  cout << (int)t;
}