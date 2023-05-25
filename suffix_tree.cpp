#include <functional>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

int getRandomId() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,10000000); // distribution in range [1, 6]

    return static_cast<int>(dist6(rng));
}

class Node
{
    public:
        string m_sub = "";      // a substring of the input string
        vector<int> m_children; // vector of child nodes
        int m_parent;
        // int endidx;
        // Default constructor for Node.
        Node()
        {
            //  The node is initialized  empty.
        }
        // Constructor for Node that takes a substring and a list of child nodes as well a starting index.
        Node(const string &sub, initializer_list<int> children, size_t startidx, int parent) : m_sub(sub), m_parent(parent)
        {
            // The child nodes of the node are inserted into the end of the children vector.
            m_children.insert(m_children.end(), children);
        }
};

class SuffixTree
{    
    public:
        std::unordered_map<int, int> m_leaves; // Key -> Node unique id , Value -> suffix index in the original string
        std::unordered_map<int, Node> m_nodes;
        size_t m_length{0};
        // Constructor for SuffixTree that takes a string as input.
        SuffixTree(const string &str)
        {
            // Initialize the tree with an empty root node.
            m_nodes.emplace(0, Node{});
            // Iterate over each character in the input string.
            for (size_t i = 0; i < str.length(); i++)
            {
                m_length = i;                    // since i becomes 0 inside `addSuffix` create a "more" global position
                addSuffix(str.substr(m_length)); // Pick a suffix string from the i position to the end of the input string and add it to the tree
            }
        }

        // Print the tree
        void visualize()
        {
            if (m_nodes.size() == 0)
            {
                cout << "<empty>\n";
                return;
            }

            function<void(int, const string &)> f;
            f = [&](int id, const string &pre)
            {
                auto children = m_nodes.at(id).m_children;
                if (children.size() == 0)
                {
                    cout << "- " << m_nodes.at(id).m_sub << " ~ " << m_leaves.at(id) << '\n';
                    return;
                }
                cout << "+ " << m_nodes.at(id).m_sub << '\n';

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

            f(0, "");
        }

private:
    void
    addSuffix(const string &suf)
    /*Adds each of the suffixes picked inside the SuffixTree constructor to the tree*/
    {
        int currentNodeKey = 0; // Current Node
        size_t i = 0;           // Current character of the suffix string
        // Check if the position of the current character is inside the suffix lenght
        while (i < suf.length())
        {
            char b = suf[i];        // Current character in the suffix string
            size_t x2 = 0;          // Index of the child node in the children list
            int childOfInterestKey; // The next node
            /*This while loop check if any of  the children of the current node begin with the current character of the suffix,
            if none do so, it creates a new child node with the remained of the suffix from the current character position*/
            while (true)
            {
                // Assign all the child nodes of the current node to the children list
                auto children = m_nodes.at(currentNodeKey).m_children;
                /* If the index of the current child node is equal to the size of the children list
                we have looked through all of the children and no match is found with their first character and the current character.
                 */
                if (x2 == children.size())
                {
                    // no matching child, create a new child node
                    Node newNode{suf.substr(i), {}, m_length, currentNodeKey}; // The remainder of the suffix from from current character position becomes the new child node
                    int newKey = getRandomId();
                    m_nodes.emplace(newKey, newNode);  // Add to nodes vector
                    m_leaves.emplace(newKey, m_length); // If no matching character is found we always add a string with $, thus we create a leaf at that node
                    m_nodes.at(currentNodeKey).m_children.push_back(newKey);
                    return;
                }
                childOfInterestKey = children[x2]; // Assign the current child node as the next node to check
                // Check if the first character of the node is the current character in the suffix string
                if (m_nodes.at(childOfInterestKey).m_sub[0] == b)
                {
                    break; // if it is, match is found -> break
                }
                x2++; // else move to the next child node and check it
            }

            /*This part of the code handles partial matching between the current suffix and an existing child node substring,
             */
            // find prefix of remaining suffix in common with child node
            auto sub2 = m_nodes.at(childOfInterestKey).m_sub; // The substring from the current character i to the end of the suffix
            size_t j = 0;
            int newKey;
            // Iterate
            while (j < sub2.size())
            {
                // if the characters of the suffix and the node substring differ
                if (suf[i + j] != sub2[j])
                {
                    // auto n3 = n2; // Assign a copy of the current child node
                    // n2 = nodes.size();
                    Node newNode{sub2.substr(0, j), {childOfInterestKey}, m_length - j, currentNodeKey}; // The remainder of the suffix from from current character position becomes the new child node
                    newKey = getRandomId();
                    m_nodes.emplace(newKey, newNode);
                    m_nodes.at(childOfInterestKey).m_sub = sub2.substr(j); // We dont change the unique id, so the leaf is carried over while the substring changes inplace, good stuff
                    m_nodes.at(childOfInterestKey).m_parent = newKey;
                    m_nodes.at(currentNodeKey).m_children[x2] = newKey;
                    break; // break and continue down the tree
                }
                j++;
            }
            i += j;                  // advance past part in common
            currentNodeKey = newKey; // continue down the tree
        }
    }
};

int main()
{
    SuffixTree tree{"BANANA$"};
    tree.visualize();
}