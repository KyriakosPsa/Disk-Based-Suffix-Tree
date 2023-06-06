#ifndef UTILS
#define UTILS

#include "utils.h"
#include <iostream>

class IdFactory
{
public:
    int createId() {
        int id = m_id;
        m_id++;
        // std::cout << id << '\n';
        return id;
    }

    int getId() {
        return m_id;
    }

private:
    int m_id{0};
};

#endif