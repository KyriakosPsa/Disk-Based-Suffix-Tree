#include <functional>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

class Node
{
    public:
        string m_sub = "";      // a substring of the input string
        vector<int> m_children; // vector of child nodes
        int m_parent;

        // Default constructor for Node.
        Node()
        {
            //  The node is initialized  empty.
        }

        // Constructor for Node that takes a substring and a list of child nodes as well a starting index.
        Node(const string &sub, initializer_list<int> children, int parent) : m_sub(sub), m_parent(parent)
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

        // Construct the suffix tree from a specified string
        void build(const string &str) {
            // Initialize the tree with an empty root node.
            if (!(m_nodes.contains(0))) {
                m_nodes.emplace(m_id, Node{});
                m_id++;
            }
            // Iterate over each character in the input string.
            for (size_t i = 0; i < str.length(); i++)
            {
                m_length = i;                    // since i becomes 0 inside `addSuffix` create a "more" global position
                addSuffix(str.substr(m_length)); // Pick a suffix string from the i position to the end of the input string and add it to the tree
            }
        }

        // Add a child with m_sub=str to all tree nodes that have only one child
        void makeExplicit(const string &str) {
            // add children to a temp map so that the iterator is not invalidated
            std::unordered_map<int, Node> tba;

            // do the root node
            Node newNode{str, {}, 0};
            tba.emplace(m_id, newNode);
            m_nodes.at(0).m_children.push_back(m_id);
            m_id++;

            for (auto &element: m_nodes) {
                // if node has only one child because it was split due to ending but no ending character was added
                if (element.second.m_children.size() == 1) {
                    Node newNode{str, {}, element.first};
                    tba.emplace(m_id, newNode);
                    element.second.m_children.push_back(m_id);
                    m_id++;
                }
            }

            // dump children into original map
            for (auto &element: tba) {
                m_nodes.emplace(element.first, element.second);
                m_leaves.emplace(element.first, -1);
            }
        }

        // Check if a prefix is unique, meaning it is not already represented within the tree structure
        bool isUnique(const string &str) {
            int currentNode = 0;
            int flag = currentNode;
            std::string remaining = str;
            while (true) {
                for(auto child: m_nodes.at(currentNode).m_children) {
                    std::string nodeStr = m_nodes.at(child).m_sub;
                    size_t searchResult;
                    if (nodeStr.length() > remaining.length()) {
                        searchResult = nodeStr.rfind(remaining, 0);
                        if (searchResult == 0) {
                            remaining = "";
                        }
                    } else {
                        searchResult = remaining.rfind(nodeStr, 0);
                        if (searchResult == 0) {
                            remaining = remaining.substr(nodeStr.length());
                        }
                    }

                    if (searchResult == 0) {
                        if (remaining.length() == 0) {
                            return false;
                        } else {
                            currentNode = child;
                            break;
                        }
                    }
                }
                if (flag == currentNode) {
                    return true;
                } else {
                    flag = currentNode;
                }
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
    int m_id{0};
    void addSuffix(const string &suf)
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
                    Node newNode{suf.substr(i), {}, currentNodeKey}; // The remainder of the suffix from from current character position becomes the new child node
                    m_nodes.emplace(m_id, newNode);  // Add to nodes vector
                    m_leaves.emplace(m_id, m_length); // If no matching character is found we always add a string with $, thus we create a leaf at that node
                    m_nodes.at(currentNodeKey).m_children.push_back(m_id);
                    m_id++;
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
            auto sub2 = m_nodes.at(childOfInterestKey).m_sub;
            size_t j = 0;
            // Iterate
            char sub1;
            while (j < sub2.size())
            {
                if (i+j == suf.length()) {
                    sub1 = '\0';
                } else {
                    sub1 = suf.at(i+j);
                }

                // if the characters of the suffix and the node substring differ
                if (sub1 != sub2[j])
                {
                    // auto n3 = n2; // Assign a copy of the current child node
                    // n2 = nodes.size();
                    Node newNode{sub2.substr(0, j), {childOfInterestKey}, currentNodeKey}; // The remainder of the suffix from from current character position becomes the new child node
                    m_nodes.emplace(m_id, newNode);
                    m_nodes.at(childOfInterestKey).m_sub = sub2.substr(j); // We dont change the unique id, so the leaf is carried over while the substring changes inplace, good stuff
                    m_nodes.at(childOfInterestKey).m_parent = m_id;
                    m_nodes.at(currentNodeKey).m_children[x2] = m_id;
                    break; // break and continue down the tree
                }
                j++;
            }
            i += j;                  // advance past part in common
            currentNodeKey = m_id; // continue down the tree
            m_id++;
        }
    }
};

int main()
{
    SuffixTree tree{};
    tree.build("BANANA");
    std::cout << std::boolalpha;
    std::cout << tree.isUnique("BAN") << '\n';
    tree.visualize();
}