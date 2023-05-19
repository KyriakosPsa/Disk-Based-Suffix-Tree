#include <functional>
#include <iostream>
#include <vector>

struct Node
{
    std::string sub = ""; // a substring of the input string
    std::vector<int> ch;  // vector of child nodes
    // Default constructor for Node.
    Node()
    {
        //  The node is initialized  empty.
    }
    // Constructor for Node that takes a substring and a list of child nodes.
    Node(const std::string &sub, std::initializer_list<int> children) : sub(sub)
    {

        // The child nodes of the node are inserted into the end of the children vector.
        ch.insert(ch.end(), children);
    }
};

struct SuffixTree
{
    std::vector<Node> nodes;
    // Constructor for SuffixTree that takes a string as input.
    SuffixTree(const std::string &str)
    {
        // Initialize the tree with an empty root node.
        nodes.push_back(Node{});
        // Iterate over each character in the input string.
        for (size_t i = 0; i < str.length(); i++)
        {
            addSuffix(str.substr(i)); // Pick a suffix string from the i position to the end of the input string and add it to the tree
        }
    }
    // Ignore for now
    void visualize()
    {
        if (nodes.size() == 0)
        {
            std::cout << "<empty>\n";
            return;
        }

        std::function<void(int, const std::string &)> f;
        f = [&](int n, const std::string &pre)
        {
            auto children = nodes[n].ch;
            if (children.size() == 0)
            {
                std::cout << "- " << nodes[n].sub << '\n';
                return;
            }
            std::cout << "+ " << nodes[n].sub << '\n';

            auto it = std::begin(children);
            if (it != std::end(children))
                do
                {
                    if (std::next(it) == std::end(children))
                        break;
                    std::cout << pre << "+-";
                    f(*it, pre + "| ");
                    it = std::next(it);
                } while (true);

            std::cout << pre << "+-";
            f(children[children.size() - 1], pre + "  ");
        };

        f(0, "");
    }

private:
    void addSuffix(const std::string &suf)
    /*Adds each of the suffixes picked inside the SuffixTree constructor to the tree*/
    {
        int n = 0;    // Current Node
        size_t i = 0; // Current character of the suffix string
        // Iterate over each character in the suffix string
        while (i < suf.length())
        {
            char b = suf[i]; // Current character in the suffix string
            int x2 = 0;      // Index of the child node in the children list
            int n2;          // The next node

            /*This while loop check if any of the children of the current node begin with the current character of the suffix,
            if none do so, it creates a new child node with the remained of the suffix from the current character position*/
            while (true)
            {
                // Assign all the child nodes of the current node to the children list
                auto children = nodes[n].ch;
                /* If the index of the current child node is equal to the size of the children list
                we have looked through all of the children and no match is found.
                 */
                if (x2 == children.size())
                {
                    // no matching child, create a new child node
                    n2 = nodes.size();                        // give the new child node its index for the children list
                    nodes.push_back(Node(suf.substr(i), {})); // The remainder of the suffix from from current character position becomes the new child node
                    nodes[n].ch.push_back(n2);                // add its index in the end of the children list
                    return;
                }
                n2 = children[x2]; // Assign the current child node as the next node to check
                // Check if the first character of the node is the current character in the suffix string
                if (nodes[n2].sub[0] == b)
                {
                    break; // if it is, match is found -> break
                }
                x2++; // else move to the next child node and check it
            }

            /*This part of the code handles partial matching between the current suffix and an existing child node substring,
             */
            // find prefix of remaining suffix in common with child node
            auto sub2 = nodes[n2].sub; // The substring from the current character i to the end of the suffix
            size_t j = 0;
            // Iterate
            while (j < sub2.size())
            {
                // if the characters of the suffix and the node substring differ
                if (suf[i + j] != sub2[j])
                {
                    auto n3 = n2; // Assign a copy of the current child node
                    n2 = nodes.size();
                    nodes.push_back(Node(sub2.substr(0, j), {n3})); // Create new node  with an empty child node, give it as substring the part in common
                    nodes[n3].sub = sub2.substr(j);                 // Update the child node of the new node with the remaining non-common substring part
                    nodes[n].ch[x2] = n2;                           // what?
                    break;                                          // break and continue down the tree
                }
                j++;
            }
            i += j; // advance past part in common
            n = n2; // continue down the tree
        }
    }
};

int main()
{
    SuffixTree("banana$").visualize();
}