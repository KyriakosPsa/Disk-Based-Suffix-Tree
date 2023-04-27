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
/*This function returns the total memory of a windows or linxus system in bytes,
it is utilized to make that value available for the calculation of t*/
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

long long int calculate_t(long long int &sequence_size, bool limit = false, int limit_memory_to = 0)
/*This function is utilized to calculate the t-threshold parameter
based on the equation t <= M - n/4)/72, where:
M -> memory in bytes
n/4 -> input string compressed to 1/4 bytes per characted
72 -> bytes per tree for a single suffix link*/
{
  long long int total_ram = get_total_ram_bytes();
  if (limit == false)
  {
    unsigned long int t = (total_ram - sequence_size / 4) / 72;
    return t;
  }
  else
  {
    // convert GB to bytes
    long long int limited_ram = limit_memory_to * pow(10, 9);
    // check if the passed limited ram is higher than the available ram
    if (limited_ram > total_ram)
    {
      throw invalid_argument("Limited RAM cannot exceed your system available RAM");
    }
    else
    {
      long long int t = (limited_ram - (sequence_size / 4)) / 72;
      return t;
    }
  }
}