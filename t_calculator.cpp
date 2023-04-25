#include <regex>
#include <iostream>
#include <string>
#include <set>
// Relevant system libaries for windoes and linux systems
// If the system is windows, include the windows.h library
#ifdef _WIN32
#include <Windows.h>
#else
// If the system is linux, include the sysinfo.h library
#include <sysinfo/sysinfo.h>
#endif

using namespace std;

// Use either the windoes or linux specific code to get the total ram
float get_total_ram_bytes()
/*This function returns the total memory of a windows or linxus system in bytes,
it is utilized to make that value available for the calculation of t*/
{
#ifdef _WIN32
  // Windows-specific code
  MEMORYSTATUSEX memory_status;
  memory_status.dwLength = sizeof(memory_status);
  GlobalMemoryStatusEx(&memory_status);
  return (float)memory_status.ullTotalPhys;
#else
  // Linux-specific code
  struct sysinfo system_info;
  sysinfo(&system_info);
  return (float)(system_info.totalram * system_info.mem_unit);
#endif
}

float calculate_t(int &sequence_size)
{
  float total_ram = get_total_ram_bytes();
  float t = (total_ram - sequence_size / 4) / 72;
  return t;
}
