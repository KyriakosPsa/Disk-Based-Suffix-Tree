#include <iostream>
#include <cmath>
#include "utils.h"
// Relevant system libaries for windows and linux systems
// If the system is windows, include the windows.h library
#ifdef _WIN32
#include <Windows.h>
#else
// If the system is linux, include the sysinfo.h library
#include <sysinfo/sysinfo.h>
#include <sys/resource.h>
#endif

// Use either the windows or linux specific code to get the total ram
size_t MemoryUtil::get_total_ram_bytes()
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

size_t MemoryUtil::calculate_t(const size_t &sequence_size,
                          const size_t &total_ram,
                          size_t limit_memory_bytes,
                          bool limit)
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
    size_t test = sequence_size / 4;
    size_t t = (total_ram - test) / 72;
    return t;
  }
  else if (limit_memory_bytes < total_ram)
  {
    size_t t = (limit_memory_bytes - (sequence_size / 4)) / 72;
    return t;
  }
  else
  {
    throw std::invalid_argument("ERROR: Limited memory cannot be set above the system's available RAM");
  }
}

int MemoryUtil::limitMemory(size_t min, size_t max) {
  #ifdef _WIN32
    if (!SetProcessWorkingSetSize(GetCurrentProcess(), min, max)) {
        std::cout << "Failed to set the working set size." << std::endl;
        return 1;
    }
    return 0;
  #else
    rlimit limit;
    getrlimit(RLIMIT_AS, &limit); // Get the current virtual memory limit

    std::cout << "Current virtual memory limit: " << limit.rlim_cur << std::endl;

    // Set a new virtual memory limit
    limit.rlim_cur = max; // 100 MB
    setrlimit(RLIMIT_AS, &limit);

    std::cout << "New virtual memory limit: " << limit.rlim_cur << std::endl;
    return 0;
  #endif
}
