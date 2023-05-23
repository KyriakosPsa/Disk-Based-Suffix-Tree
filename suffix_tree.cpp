#include <functional>
#include <iostream>
#include <vector>
#include <random>


using namespace std;

struct Node
{
    string m_sub = ""; // a substring of the input string
    vector<std::string> m_children;  // vector of child nodes
    size_t m_startidx;
    // int endidx;
    // Default constructor for Node.
    Node() : m_startidx{0}
    {
        //  The node is initialized  empty.
    }
    // Constructor for Node that takes a substring and a list of child nodes as well a starting index.
    Node(const string &sub, initializer_list<std::string> children, size_t startidx) : m_sub(sub), m_startidx(startidx)
    {
        // The child nodes of the node are inserted into the end of the children vector.
        m_children.insert(m_children.end(), children);
    }

    void addChild(std::string &child) {
        m_children.insert(m_children.end(), child);
    }
};

std::string getRandomId() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,10000000); // distribution in range [1, 6]

    return std::to_string(dist6(rng));
}

struct SuffixTree
{
    std::unordered_map<std::string, Node> nodes;
    size_t charpos{0};
    // Constructor for SuffixTree that takes a string as input.
    SuffixTree(const string &str)
    {
        // Initialize the tree with an empty root node.
        nodes.emplace("root", Node{});
        // Iterate over each character in the input string.
        for (size_t i = 0; i < str.length(); i++)
        {
            charpos = i;                    // since i becomes 0 inside `addSuffix` create a "more" global position
            addSuffix(str.substr(charpos)); // Pick a suffix string from the i position to the end of the input string and add it to the tree
        }
    }
    // // Ignore for now
    void visualize()
    {
        if (nodes.size() == 0)
        {
            cout << "<empty>\n";
            return;
        }

        function<void(std::string, const string &)> f;
        f = [&](std::string id, const string &pre)
        {
            auto children = nodes.at(id).m_children;
            if (children.size() == 0)
            {
                cout << "- " << nodes.at(id).m_sub << '\n';
                return;
            }
            cout << "+ " << nodes.at(id).m_sub << '\n';

            auto it = begin(children);
            if (it != end(children))
                do
                {
                    if (next(it) == end(children))
                        break;
                    cout << pre << "+-";
                    f(*it, pre + "| ");
                    it = next(it);
                } while (true);

            cout << pre << "+-";
            f(children[children.size() - 1], pre + "  ");
        };

        f("root", "");
    }

    // void suffixIndex()
    // // Retun the indeces of the suffixes in the input string
    // {
    //     // Iterate over the nodes
    //     for (size_t k = 0; k < nodes.size() - 1; k++)
    //     {
    //         // Pick those without children, aka leaf nodes
    //         if (nodes[k].ch.empty())
    //         {
    //             cout << "Index: " << nodes[k].startidx << ", Leaf Node Substring: " << nodes[k].sub << "\n";
    //         }
    //     }
    // }

private:
    void
    addSuffix(const string &suf)
    /*Adds each of the suffixes picked inside the SuffixTree constructor to the tree*/
    {
        std::string currentNodeKey = "root";    // Current Node
        size_t i = 0; // Current character of the suffix string
        // Check if the position of the current character is inside the suffix lenght
        while (i < suf.length())
        {
            char b = suf[i]; // Current character in the suffix string
            size_t x2 = 0;      // Index of the child node in the children list
            std::string childOfInterestKey;                         // The next node
            /*This while loop check if any of  the children of the current node begin with the current character of the suffix,
            if none do so, it creates a new child node with the remained of the suffix from the current character position*/
            while (true)
            {
                // Assign all the child nodes of the current node to the children list
                auto children = nodes.at(currentNodeKey).m_children;
                /* If the index of the current child node is equal to the size of the children list
                we have looked through all of the children and no match is found with their first character and the current character.
                 */
                if (x2 == children.size())
                {
                    // no matching child, create a new child node
                    Node newNode{suf.substr(i), {}, charpos};    // The remainder of the suffix from from current character position becomes the new child node
                    std::string newKey = getRandomId();
                    nodes.emplace(newKey, newNode);       // Add to nodes vector
                    nodes.at(currentNodeKey).m_children.push_back(newKey);
                    return;
                }
                childOfInterestKey = children[x2]; // Assign the current child node as the next node to check
                // Check if the first character of the node is the current character in the suffix string
                if (nodes.at(childOfInterestKey).m_sub[0] == b)
                {
                    break; // if it is, match is found -> break
                }
                x2++; // else move to the next child node and check it
            }

            /*This part of the code handles partial matching between the current suffix and an existing child node substring,
             */
            // find prefix of remaining suffix in common with child node
            auto sub2 = nodes.at(childOfInterestKey).m_sub; // The substring from the current character i to the end of the suffix
            size_t j = 0;
            // Iterate
            while (j < sub2.size())
            {
                // if the characters of the suffix and the node substring differ
                if (suf[i + j] != sub2[j])
                {
                    // auto n3 = n2; // Assign a copy of the current child node
                    // n2 = nodes.size();
                    Node newNode{sub2.substr(0, j), {childOfInterestKey}, charpos - j}; // The remainder of the suffix from from current character position becomes the new child node
                    std::string newKey = getRandomId();
                    nodes.emplace(newKey, newNode);
                    nodes.at(childOfInterestKey).m_sub = sub2.substr(j);
                    nodes.at(currentNodeKey).m_children[x2] = newKey;
                    break;                                                       // break and continue down the tree
                }
                j++;
            }
            i += j; // advance past part in common
            currentNodeKey = childOfInterestKey; // continue down the tree
        }
    }
};
int main()
{
    SuffixTree tree{"BANANA$"};
    tree.visualize();
}