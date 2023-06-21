#ifndef UTILS
#define UTILS

#include "utils.h"
#include <iostream>

class IdFactory
{
public:
    int createId()
    {
        int id = m_id;
        m_id++;
        // std::cout << id << '\n';
        return id;
    }

    int getId()
    {
        return m_id;
    }

private:
    int m_id{0};
};

class MemoryUtil
{
public:
    size_t calculate_t(const size_t &sequence_size, const size_t &total_ram, size_t limit_memory_bytes = 0, bool limit = false);
    size_t get_total_ram_bytes();
    int limitMemoryWindows(size_t min, size_t max);
    int limitMemoryLinux(size_t max);
};

#endif