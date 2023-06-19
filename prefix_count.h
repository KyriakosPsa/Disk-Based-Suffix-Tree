#ifndef PREFIX_COUNT
#define PREFIX_COUNT

#include <vector>
#include <string>

class PrefixCounter {
    private:
    int m_t{};
    std::string m_sequence{};
    size_t m_sequenceLength{};
    std::vector<std::string> m_extendVector{""};
    std::vector<std::string> m_prefixVector;
    char m_alphabet[4]{ 'A', 'G', 'C', 'T' };

    int evalInequality(size_t currLength, size_t prevLength);

    void extendPrefixes();

    void clearEmpties();

    int countSubstring(std::string target);

    public:
    PrefixCounter(int t, std::string sequence);

    void runMultiPass();

    std::vector<std::string>& getPrefixes();

};
#endif