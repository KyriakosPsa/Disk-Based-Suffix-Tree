#include <vector>
#include <string>

#ifndef MY_HEADER_H
#define MY_HEADER_H

// Function declaration(s)

// Memory managment utilites
long long int get_total_ram_bytes();

long long int calculate_t(long long int &sequence_size,
                          long long int &total_ram,
                          long long int limit_memory_bytes,
                          bool limit = false);

float calculate_t(int &sequence_size, bool limit = false, int limit_memory_to = 2);

#endif