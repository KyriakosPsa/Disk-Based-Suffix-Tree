#include <functional>
#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    string sub = ""; // a substring of the input string
    vector<size_t> ch;  // vector of child nodes
    size_t startidx;
    // int endidx;
    // Default constructor for Node.
    Node() : startidx{0}
    {
        //  The node is initialized  empty.
    }
    // Constructor for Node that takes a substring and a list of child nodes as well a starting index.
    Node(const string &sub, initializer_list<size_t> children, size_t startidx) : sub(sub), startidx(startidx)
    {
        // The child nodes of the node are inserted into the end of the children vector.
        ch.insert(ch.end(), children);
    }
};

struct SuffixTree
{
    vector<Node> nodes;
    size_t charpos{0};
    // Constructor for SuffixTree that takes a string as input.
    SuffixTree(const string &str)
    {
        // Initialize the tree with an empty root node.
        nodes.push_back(Node{});
        // Iterate over each character in the input string.
        for (size_t i = 0; i < str.length(); i++)
        {
            charpos = i;                    // since i becomes 0 inside `addSuffix` create a "more" global position
            addSuffix(str.substr(charpos)); // Pick a suffix string from the i position to the end of the input string and add it to the tree
        }
    }
    // Ignore for now
    void visualize()
    {
        if (nodes.size() == 0)
        {
            cout << "<empty>\n";
            return;
        }

        function<void(size_t, const string &)> f;
        f = [&](size_t n, const string &pre)
        {
            auto children = nodes[n].ch;
            if (children.size() == 0)
            {
                cout << "- " << nodes[n].sub << '\n';
                return;
            }
            cout << "+ " << nodes[n].sub << '\n';

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

    void suffixIndex()
    // Retun the indeces of the suffixes in the input string
    {
        // Iterate over the nodes
        for (size_t k = 0; k < nodes.size() - 1; k++)
        {
            // Pick those without children, aka leaf nodes
            if (nodes[k].ch.empty())
            {
                cout << "Index: " << nodes[k].startidx << ", Leaf Node Substring: " << nodes[k].sub << "\n";
            }
        }
    }

private:
    void
    addSuffix(const string &suf)
    /*Adds each of the suffixes picked inside the SuffixTree constructor to the tree*/
    {
        size_t n = 0;    // Current Node
        size_t i = 0; // Current character of the suffix string
        // Check if the position of the current character is inside the suffix lenght
        while (i < suf.length())
        {
            char b = suf[i]; // Current character in the suffix string
            size_t x2 = 0;      // Index of the child node in the children list
            size_t n2;          // The next node
            /*This while loop check if any of  the children of the current node begin with the current character of the suffix,
            if none do so, it creates a new child node with the remained of the suffix from the current character position*/
            while (true)
            {
                // Assign all the child nodes of the current node to the children list
                auto children = nodes[n].ch;
                /* If the index of the current child node is equal to the size of the children list
                we have looked through all of the children and no match is found with their first character and the current character.
                 */
                if (x2 == children.size())
                {
                    // no matching child, create a new child node
                    n2 = nodes.size();                                 // give the new child node its index for the children list
                    nodes.push_back(Node(suf.substr(i), {}, charpos)); // The remainder of the suffix from from current character position becomes the new child node
                    nodes[n].ch.push_back(n2);                         // add its index in the end of the children list
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
                    nodes.push_back(Node(sub2.substr(0, j), {n3}, charpos - j)); // Create new node  with an empty child node, give it as substring the part in common
                    nodes[n3].sub = sub2.substr(j);                              // Update the child node of the new node with the remaining non-common substring part
                    nodes[n].ch[x2] = n2;                                        // what?
                    break;                                                       // break and continue down the tree
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
    SuffixTree("BANANA$").suffixIndex();
}