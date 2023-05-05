#include <vector>
#include <string>

#ifndef MY_HEADER_H
#define MY_HEADER_H

// Function declaration(s)
using namespace std;

// Memory managment utilites
long long int get_total_ram_bytes();

long long int calculate_t(long long int &sequence_size,
                          long long int &total_ram,
                          long long int limit_memory_bytes,
                          bool limit = false);

// File handling utilites
string readFile(string &fileName);
#endif