#include <vector>
#include <string>
#include <algorithm>
#include "prefix_count.h"

PrefixCounter::PrefixCounter(int t, std::string sequence) 
    : m_t{ t }, m_sequence{sequence}, m_sequenceLength{sequence.length()} 
    {
    
    }
int m_t{};
std::string m_sequence{};
size_t m_sequenceLength{};
std::vector<std::string> m_extendVector{ "" };
std::vector<std::string> m_prefixVector;
char m_alphabet[4]{ 'A', 'G', 'C', 'T' };

int  PrefixCounter::evalInequality(size_t currLength, size_t prevLength)
{
    size_t sum{0};
    for (size_t i = 1; i <= (currLength - prevLength); i++)
    {
        sum = sum + (4 ^ i);
    }

    return m_extendVector.size() * sum;
}

void  PrefixCounter::extendPrefixes()
{
    size_t prefixes{m_extendVector.size()};

    for (long long unsigned int i = 0; i < prefixes; i++)
    {
        // modify current prefix in place for the first letter.
        // put the rest at the end of extendVector;
        m_extendVector.push_back(m_extendVector[i] + m_alphabet[1]);
        m_extendVector.push_back(m_extendVector[i] + m_alphabet[2]);
        m_extendVector.push_back(m_extendVector[i] + m_alphabet[3]);
        m_extendVector[i] = m_extendVector[i] + m_alphabet[0];
    }
}

void  PrefixCounter::clearEmpties()
{
    auto isEmpty = [](const std::string &s) {
        return (s.length() == 0);
    };
    m_extendVector.erase(std::remove_if(m_extendVector.begin(), m_extendVector.end(), isEmpty), m_extendVector.end());
}

int  PrefixCounter::countSubstring(std::string target)
{
    int counter{ 0 };
    size_t searchLen = target.length();
    
    for (size_t i = 0; i < m_sequenceLength; i++) {
        int result = m_sequence.compare(i, searchLen, target);
            if (result == 0) {
                counter+= 1;
                if (counter == m_t) {
                    return counter;
                }
            }
    }
    return counter;
}

void  PrefixCounter::runMultiPass()
{
    m_extendVector.reserve(10000);

    size_t length{ 0 };
    size_t prevlength{ 0 };

    while (true)
    {
        while (evalInequality(length, prevlength) <= m_t)
        {
            length += 1;
            extendPrefixes();
            size_t index{ 0 };
            for (std::string prefix : m_extendVector)
            {
                int freq = countSubstring(prefix);
                if (freq < m_t)
                {
                    m_prefixVector.push_back(prefix);
                    m_extendVector[index] = "";
                }
                index += 1;
            }

            clearEmpties();
            if (m_extendVector.size() == 0)
            {
                break;
            }
        }
        prevlength = length;
        if (m_extendVector.size() == 0)
        {
            break;
        }
    }
}
std::vector<std::string>& PrefixCounter::getPrefixes() {
    runMultiPass();
    return m_prefixVector;
}