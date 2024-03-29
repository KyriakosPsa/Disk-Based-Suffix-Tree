#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include "node.h"
#include "suffix_tree.h"
#include "utils.h"

int SuffixTree::m_globalId = 0;

std::vector<std::string> splitByDelimiter(const std::string &str, char delimiter)
{
    std::vector<std::string> segments;
    std::string segment;
    std::stringstream stream(str);
    while (std::getline(stream, segment, delimiter))
    {
        segments.push_back(segment);
    }
    return segments;
}

SuffixTree::SuffixTree()
{
}

SuffixTree::SuffixTree(std::vector<std::pair<int, Node>> nodes, size_t length, int rootId)
{
    // TODO fix unused and wrong properties length, idFactory
    m_length = length;
    m_rootId = rootId;
    for (auto node : nodes)
    {
        m_nodes.insert(node);
    }
}

SuffixTree::SuffixTree(std::ifstream &archive)
{
    std::string temp;
    std::getline(archive, temp);
    m_leaves = deserializeLeaves(temp);
    std::getline(archive, temp);
    m_nodes = deserializeNodes(temp);
    std::getline(archive, temp);
    m_length = stringToSize(temp);
    std::getline(archive, temp);
    m_rootId = stoi(temp);
    archive.close();
}

// Construct the suffix tree from a specified string
SuffixTree::SuffixTree(const std::string &str, const std::string &prefix)
{
    m_length = str.length();
    // Initialize the tree with an empty root node.
    // root parent = -1
    int id = createId();
    m_nodes.emplace(id, Node{prefix, {}, -1});
    m_rootId = id;

    // Iterate over each character in the input string.
    for (size_t i = 0; i < str.length(); i++)
    {
        addSuffix(str.substr(i)); // Pick a suffix string from the i position to the end of the input string and add it to the tree
    }
}

// // Add a child with m_sub=str to all tree nodes that have only one child
// void SuffixTree::makeExplicit(const string &str)
// {
//     // add children to a temp map so that the iterator is not invalidated
//     std::unordered_map<int, Node> tba;

//     // do the root node
//     Node newNode{str, {}, 0};
//     int id = (*m_idFactory).createId();
//     tba.emplace(id, newNode);
//     m_nodes.at(m_rootId).m_children.push_back(id);

//     for (auto &element : m_nodes)
//     {
//         // if node has only one child because it was split due to ending but no ending character was added
//         if (element.second.m_children.size() == 1)
//         {
//             Node newNode{str, {}, element.first};
//             int id = (*m_idFactory).createId();
//             tba.emplace(id, newNode);
//             element.second.m_children.push_back(id);
//         }
//     }

//     // dump children into original map
//     for (auto &element : tba)
//     {
//         m_nodes.emplace(element.first, element.second);
//     }
// }

// Check if a prefix is unique, meaning it is not already represented within the tree structure
// bool SuffixTree::isUnique(const string &str)
// {
//     int currentNode = m_rootId;
//     int flag = currentNode;
//     std::string remaining = str;
//     while (true)
//     {
//         for (auto child : m_nodes.at(currentNode).m_children)
//         {
//             std::string nodeStr = m_nodes.at(child).m_sub;
//             size_t searchResult;
//             if (nodeStr.length() > remaining.length())
//             {
//                 searchResult = nodeStr.rfind(remaining, 0);
//                 if (searchResult == 0)
//                 {
//                     remaining = "";
//                 }
//             }
//             else
//             {
//                 searchResult = remaining.rfind(nodeStr, 0);
//                 if (searchResult == 0)
//                 {
//                     remaining = remaining.substr(nodeStr.length());
//                 }
//             }

//             if (searchResult == 0)
//             {
//                 if (remaining.length() == 0)
//                 {
//                     return false;
//                 }
//                 else
//                 {
//                     currentNode = child;
//                     break;
//                 }
//             }
//         }
//         if (flag == currentNode)
//         {
//             return true;
//         }
//         else
//         {
//             flag = currentNode;
//         }
//     }
// }

void SuffixTree::makeLeaves()
{
    for (auto &element : m_nodes) // Check all the nodes
    {
        if (element.second.m_children.empty()) // if a node has no children -> leaf node
        {
            size_t parentlen{0}; // Initialize the length of the path from the leaf to root
            int parentNode{m_rootId};
            parentlen += element.second.m_sub.length(); // Add the length of the leaf node substring
            parentNode = element.second.m_parent;       // Pass the first parent
            while (parentNode >= 0)                     // Iterate over all available parents
            {
                parentlen += m_nodes.at(parentNode).m_sub.length(); // Extended the length of the path based on each parent substring
                parentNode = m_nodes.at(parentNode).m_parent;
            }
            m_leaves.emplace(element.first, m_length - parentlen); // Add the leaf suffix index to the node without children we identified in the if statement
        }
    }
}

// Print the tree
void SuffixTree::visualize()
{
    if (m_nodes.size() == 0)
    {
        std::cout << "<empty>\n";
        return;
    }

    std::function<void(int, const std::string &)> f;
    f = [&](int id, const std::string &pre)
    {
        auto children = m_nodes.at(id).m_children;
        if (children.size() == 0)
        {
            std::cout << "- " << m_nodes.at(id).m_sub << " ~ " << m_leaves.at(id) << '\n';
            return;
        }
        std::cout << "+ " << m_nodes.at(id).m_sub << '\n';

        auto it = begin(children);
        if (it != end(children))
            do
            {
                if (next(it) == end(children))
                    break;
                std::cout << pre << "+-";
                f(*it, pre + "| ");
                it = next(it);
            } while (true);

        std::cout << pre << "+-";
        f(children[children.size() - 1], pre + "  ");
    };

    f(0, "");
}

void SuffixTree::visualizeNoLeaves()
{
    if (m_nodes.size() == 0)
    {
        std::cout << "<empty>\n";
        return;
    }

    std::function<void(int, const std::string &)> f;
    f = [&](int id, const std::string &pre)
    {
        auto children = m_nodes.at(id).m_children;
        if (children.size() == 0)
        {
            std::cout << "- " << m_nodes.at(id).m_sub << '\n';
            return;
        }
        std::cout << "+ " << m_nodes.at(id).m_sub << '\n';

        auto it = begin(children);
        if (it != end(children))
            do
            {
                if (next(it) == end(children))
                    break;
                std::cout << pre << "+-";
                f(*it, pre + "| ");
                it = next(it);
            } while (true);

        std::cout << pre << "+-";
        f(children[children.size() - 1], pre + "  ");
    };

    f(m_rootId, "");
}

void SuffixTree::visualizeNoLeaves(std::string &fileName)
{
    std::ofstream outputFile;
    outputFile.open(fileName);
    if (m_nodes.size() == 0)
    {
        outputFile << "<empty>\n";
        return;
    }

    std::function<void(int, const std::string &)> f;
    f = [&](int id, const std::string &pre)
    {
        auto children = m_nodes.at(id).m_children;
        if (children.size() == 0)
        {
            outputFile << "- " << m_nodes.at(id).m_sub << '\n';
            return;
        }
        outputFile << "+ " << m_nodes.at(id).m_sub << '\n';

        auto it = begin(children);
        if (it != end(children))
            do
            {
                if (next(it) == end(children))
                    break;
                outputFile << pre << "+-";
                f(*it, pre + "| ");
                it = next(it);
            } while (true);

        outputFile << pre << "+-";
        f(children[children.size() - 1], pre + "  ");
    };

    f(m_rootId, "");
    outputFile.close();
}

void SuffixTree::getAllChildren(int node, std::vector<std::pair<int, Node>> &children)
{
    for (auto child : m_nodes.at(node).m_children)
    {
        children.push_back(std::make_pair(child, m_nodes.at(child)));
        this->getAllChildren(child, children);
    }
}

void SuffixTree::copyNodes(std::vector<std::pair<int, Node>> &children)
{
    std::vector<int> deletedNodes{};
    for (std::pair<int, Node> child : children)
    {
        if (child.second.m_sub.ends_with('$'))
        {
            child.second.m_sub.pop_back();
        }
        if (child.second.m_sub.length() > 0)
        {
            m_nodes.emplace(child.first, child.second);
        }
        else
        {
            std::vector<int> newChildren = m_nodes.at(child.second.m_parent).m_children;
            newChildren.erase(std::remove(newChildren.begin(), newChildren.end(), child.first), newChildren.end());
            m_nodes.at(child.second.m_parent).m_children = newChildren;
        }
    }
}

std::vector<int> SuffixTree::queryPrefix(const std::string &str)
{
    int currentNode = m_rootId;
    int flag = currentNode;
    std::string remaining = str;
    std::vector<int> involvedNodes;
    while (true)
    {
        for (auto child : m_nodes.at(currentNode).m_children)
        {
            std::string nodeStr = m_nodes.at(child).m_sub;
            size_t searchResult;
            if (nodeStr.length() > remaining.length())
            {
                searchResult = nodeStr.rfind(remaining, 0);
                if (searchResult == 0)
                {
                    remaining = "";
                }
            }
            else
            {
                searchResult = remaining.rfind(nodeStr, 0);
                if (searchResult == 0)
                {
                    remaining = remaining.substr(nodeStr.length());
                }
            }

            if (searchResult == 0)
            {
                if (remaining.length() == 0)
                {
                    involvedNodes.push_back(child);
                    return involvedNodes;
                }
                else
                {
                    involvedNodes.push_back(child);
                    currentNode = child;
                    break;
                }
            }
        }
        if (flag == currentNode)
        {
            // std::cout << "Not found in tree: " << remaining << '\n';
            return involvedNodes;
        }
        else
        {
            flag = currentNode;
        }
    }
}

void SuffixTree::deleteNode(int node)
{
    m_nodes.erase(node);
}

void SuffixTree::removeAsChild(int node)
{
    int parent = m_nodes.at(node).m_parent;
    if (m_nodes.contains(parent))
    {
        std::vector<int> *children = &m_nodes.at(parent).m_children;
        std::vector<int>::iterator position = std::find((*children).begin(), (*children).end(), node);
        if (position != (*children).end()) // == myVector.end() means the element was not found
            (*children).erase(position);
    }
}

void SuffixTree::deleteNode(std::vector<std::pair<int, Node>> nodes)
{
    for (auto node : nodes)
    {
        deleteNode(node.first);
    }
}

size_t SuffixTree::stringToSize(std::string &str)
{
    std::stringstream stream(str);
    size_t output;
    stream >> output;

    return output;
}

std::unordered_map<int, Node> SuffixTree::deserializeNodes(std::string &line)
{
    std::vector<std::string> segments = splitByDelimiter(line, '.');
    std::unordered_map<int, Node> nodes;

    for (size_t i = 0; i < segments.size(); i += 2)
    {
        nodes.emplace(stoi(segments[i]), Node(segments[i + 1]));
    }
    return nodes;
}

void SuffixTree::serialize(const std::string &fileName)
{
    std::ofstream archive{fileName};
    archive << serializeLeaves() << '\n';
    archive << serializeNodes() << '\n';
    archive << m_length << '\n';
    archive << m_rootId << '\n';
    archive.close();
}

void SuffixTree::addSuffix(const std::string &suf)
/*Adds each of the suffixes picked inside the SuffixTree constructor to the tree*/
{
    int currentNodeKey = m_rootId; // Current Node
    size_t i = 0;                  // Current character of the suffix string
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
            // if (!m_nodes.contains(currentNodeKey)) {
            //     std::cout << currentNodeKey << '\n';
            // }
            // Assign all the child nodes of the current node to the children list
            auto children = m_nodes.at(currentNodeKey).m_children;
            /* If the index of the current child node is equal to the size of the children list
            we have looked through all of the children and no match is found with their first character and the current character.
                */
            if (x2 == children.size())
            {
                int id = createId();
                // no matching child, create a new child node
                Node newNode{suf.substr(i), {}, currentNodeKey}; // The remainder of the suffix from from current character position becomes the new child node
                m_nodes.emplace(id, newNode);                    // Add to nodes vector
                m_nodes.at(currentNodeKey).m_children.push_back(id);
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
        bool didSplit{false};
        while (j < sub2.size())
        {
            if (i + j == suf.length())
            {
                sub1 = '\0';
            }
            else
            {
                sub1 = suf.at(i + j);
            }
            // if the characters of the suffix and the node substring differ
            if (sub1 != sub2[j])
            {
                // auto n3 = n2; // Assign a copy of the current child node
                // n2 = nodes.size();
                int id = createId();
                Node newNode{sub2.substr(0, j), {childOfInterestKey}, currentNodeKey}; // The remainder of the suffix from from current character position becomes the new child node
                m_nodes.emplace(id, newNode);
                m_nodes.at(childOfInterestKey).m_sub = sub2.substr(j); // We dont change the unique id, so the leaf is carried over while the substring changes inplace, good stuff
                m_nodes.at(childOfInterestKey).m_parent = id;
                m_nodes.at(currentNodeKey).m_children[x2] = id;
                currentNodeKey = id;
                didSplit = true;
                break; // break and continue down the tree
            }
            j++;
        }
        i += j; // advance past part in common
        if (!didSplit)
        {
            currentNodeKey = childOfInterestKey;
        }
    }
}

std::string SuffixTree::serializeLeaves()
{
    std::string gen{""};
    for (auto i : m_leaves)
    {
        gen += std::to_string(i.first);
        gen += '.';
        gen += std::to_string(i.second);
        gen += '.';
    }
    return gen;
}

std::unordered_map<int, int> SuffixTree::deserializeLeaves(std::string &line)
{
    std::vector<std::string> segments = splitByDelimiter(line, '.');
    std::unordered_map<int, int> leaves;
    for (size_t i = 0; i < segments.size(); i += 2)
    {
        leaves.emplace(stoi(segments[i]), stoi(segments[i + 1]));
    }
    return leaves;
}

std::string SuffixTree::serializeNodes()
{
    std::string gen{""};
    for (auto i : m_nodes)
    {
        gen += std::to_string(i.first);
        gen += '.';
        gen += i.second.serialize();
        gen += '.';
    }
    return gen;
}

int SuffixTree::countLeaves()
{
    return m_leaves.size();
}

void SuffixTree::merge(SuffixTree &rightTree)
{
    // the current tree is the left tree, the incoming tree is the right tree.
    mergeChildren(m_rootId, rightTree.m_rootId, rightTree);
}

void SuffixTree::testId(int nodeId, int lineId)
{
    try
    {
        Node newNode = this->m_nodes.at(nodeId);
    }
    catch (std::exception &e)
    {
        std::cout << "Out of range at line: " << lineId << " " << e.what() << " at.() " << nodeId << std::endl;
        // std::cout << e.what() << " " << lineId << std::endl;
    }
}

void SuffixTree::splitRoot(std::string prefix)
{
    Node rootNode = m_nodes.at(m_rootId);
    if (rootNode.m_sub.ends_with('$'))
    {
        rootNode.m_sub.pop_back();
    }
    if (rootNode.m_sub.length() > prefix.length())
    {
        std::string commonPrefix = findCommonPrefix(rootNode.m_sub, prefix);
        int newId = createId();
        Node newNode{rootNode.m_sub.substr(commonPrefix.length()), rootNode.m_children, m_rootId};
        m_nodes.emplace(newId, newNode);

        rootNode.m_sub = commonPrefix;
        rootNode.m_children = std::vector<int>{newId};
    }
}

void SuffixTree::removeDollarChildren(int nodeId)
{
    std::vector<int> newChildren{};
    for (int child : this->m_nodes.at(nodeId).m_children)
    {
        std::string nodeString = this->m_nodes.at(child).m_sub;
        if (nodeString.ends_with('$'))
        {
            nodeString.pop_back();
        }
        if (nodeString.length() > 0)
        {
            this->m_nodes.at(child).m_sub = nodeString;
            newChildren.push_back(child);
        }
        else
        {
            this->m_nodes.erase(child);
        }
    }
    this->m_nodes.at(nodeId).m_children = newChildren;
}

int SuffixTree::createId() {
    int id = m_globalId;
    m_globalId += 1;
    return id;
}

void SuffixTree::mergeChildren(int leftNodeId, int rightNodeId, SuffixTree &rightTree)
{
    std::string rightString;
    std::string leftString;
    std::string subStr;

    // remove children that are just the "$" string.
    this->removeDollarChildren(leftNodeId);
    rightTree.removeDollarChildren(rightNodeId);

    std::vector<int> rightChildren = rightTree.m_nodes.at(rightNodeId).m_children;
    std::vector<int> leftChildren = this->m_nodes.at(leftNodeId).m_children;
    std::vector<int> pendingRight = rightChildren;
    for (int leftChild : this->m_nodes.at(leftNodeId).m_children)
    {
        leftString = this->m_nodes.at(leftChild).m_sub;
        for (int rightChild: rightChildren) {
            rightString = rightTree.m_nodes.at(rightChild).m_sub;
            std::string commonPrefix = findCommonPrefix(leftString, rightString);
            if ((commonPrefix.length() == leftString.length()) && (commonPrefix.length() == rightString.length())) {
                mergeChildren(leftChild, rightChild, rightTree);
                pendingRight.erase(std::remove(pendingRight.begin(), pendingRight.end(), rightChild), pendingRight.end());
            }
            else if (commonPrefix.length() > 0)
            {
                if (leftString.substr(commonPrefix.length()).length() == 0)
                {
                    Node rightNew{rightString.substr(commonPrefix.length()), rightTree.m_nodes.at(rightChild).m_children, leftChild};

                    this->m_nodes.emplace(rightChild, rightNew);

                    std::vector<std::pair<int, Node>> children;
                    rightTree.getAllChildren(rightChild, children);
                    this->copyNodes(children);
                    pendingRight.erase(std::remove(pendingRight.begin(), pendingRight.end(), rightChild), pendingRight.end());
                    m_nodes.at(leftChild).m_children.push_back(rightChild);
                    break;
                }
                else if (rightString.substr(commonPrefix.length()).length() == 0)
                {
                    int idLeft = createId();
                    std::string leftNewString = leftString.substr(commonPrefix.length());
                    Node leftNew{leftNewString, m_nodes.at(leftChild).m_children, leftChild};
                    this->m_nodes.emplace(idLeft, leftNew);
                    this->m_nodes.emplace(rightChild, rightTree.m_nodes.at(rightChild));
                    this->m_nodes.at(rightChild).m_parent = leftChild;
                    std::vector<std::pair<int, Node>> children;
                    rightTree.getAllChildren(rightChild, children);
                    for (std::pair<int, Node> child : children)
                    {
                        if (child.second.m_parent == rightChild)
                        {
                            child.second.m_parent = leftChild;
                        }
                    }
                    this->copyNodes(children);
                    pendingRight.erase(std::remove(pendingRight.begin(), pendingRight.end(), rightChild), pendingRight.end());
                    m_nodes.at(leftChild).m_children = std::vector<int>{idLeft, rightChild};
                    m_nodes.at(leftChild).m_sub = commonPrefix;
                    break;
                }
                else
                {
                    // Create a new node id to split left
                    int idLeft = createId();

                    // m_sub is the substr from the common prefix length to the end of the original string
                    // m_children are the children of each original node, now moved to be children of the second half of the split node.
                    // m_parent is the left node id. The right tree will be discarded.
                    Node leftNew{leftString.substr(commonPrefix.length()), m_nodes.at(leftChild).m_children, leftChild};
                    this->m_nodes.emplace(idLeft, leftNew);

                    // left node is edited
                    // m_sub is the common prefix
                    // m_children is only the two new nodes, the other children have been moved to leftNew.
                    m_nodes.at(leftChild).m_children = std::vector<int>{idLeft, rightChild};
                    m_nodes.at(leftChild).m_sub = commonPrefix;

                    // create new right node with id from the other tree so all children point to it, and with the substring as m_sub
                    Node rightNew{rightString.substr(commonPrefix.length()), rightTree.m_nodes.at(rightChild).m_children, leftChild};
                    this->m_nodes.emplace(rightChild, rightNew);
                    std::vector<std::pair<int, Node>> children;
                    rightTree.getAllChildren(rightChild, children);
                    this->copyNodes(children);
                    pendingRight.erase(std::remove(pendingRight.begin(), pendingRight.end(), rightChild), pendingRight.end());
                    break;
                }
            }
        }
        rightChildren = pendingRight;
    }

    for (int rightChild : pendingRight)
    {
        this->m_nodes.emplace(rightChild, rightTree.m_nodes.at(rightChild));
        this->m_nodes.at(rightChild).m_parent = leftNodeId;
        this->m_nodes.at(leftNodeId).m_children.push_back(rightChild);

        std::vector<std::pair<int, Node>> children;
        rightTree.getAllChildren(rightChild, children);
        this->copyNodes(children);
    }
}

SuffixTree *splitTree(SuffixTree &tree, const std::string &str, std::vector<int> prefixLocation)
{
    std::string rootString{""};
    int finalNode = prefixLocation.back();
    std::string pathString;
    for (int node : prefixLocation)
    {
        pathString += tree.m_nodes.at(node).m_sub;
    }

    if (pathString.length() > str.length())
    {
        rootString = pathString;
    } else if (pathString.length() == str.length()) {
        rootString = str;
    }

    if (rootString.length() > 0) {
        // add new root node and remove it as child from parent in old tree
        Node newRoot{rootString, tree.m_nodes.at(finalNode).m_children, -1};
        std::vector<std::pair<int, Node>> newNodes;
        newNodes.push_back(std::make_pair(finalNode, newRoot));
        tree.removeAsChild(finalNode);

        tree.getAllChildren(finalNode, newNodes);
        tree.deleteNode(newNodes);
        return new SuffixTree(newNodes, str.length(), finalNode);
    }
    
    return new SuffixTree("");
}
