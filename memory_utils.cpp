#include <iostream>
#include <cmath>
// Relevant system libaries for windows and linux systems
// If the system is windows, include the windows.h library
#ifdef _WIN32
#include <Windows.h>
#else
// If the system is linux, include the sysinfo.h library
#include <sysinfo/sysinfo.h>
#endif

using namespace std;

// Use either the windows or linux specific code to get the total ram
long long int get_total_ram_bytes()
/**
 * Returns the total available memory of a windows or linxus system in bytes
 */
{
#ifdef _WIN32
  // Windows-specific code
  MEMORYSTATUSEX memory_status;
  memory_status.dwLength = sizeof(memory_status);
  GlobalMemoryStatusEx(&memory_status);
  return memory_status.ullTotalPhys;
#else
  // Linux-specific code
  struct sysinfo system_info;
  sysinfo(&system_info);
  return (system_info.totalram * system_info.mem_unit);
#endif
}

long long int calculate_t(long long int &sequence_size,
                          long long int &total_ram,
                          long long int limit_memory_bytes,
                          bool limit = false)
/**
 * Computes t, a crucial threshold for the memory mamagement of the Algorithm
 *
 * @param sequence_size The size of the inputdna sequence.
 * @param total_ram The total available system RAM
 * @param limit whether or not to allow for memory limitation
 * @param limit_memory_bytes memory limit in GB
 */
{
  if (limit == false)
  {
    long long int t = (total_ram - sequence_size / 4) / 72;
    return t;
  }
  else
  {
    long long int t = (limit_memory_bytes - (sequence_size / 4)) / 72;
    return t;
  }
}