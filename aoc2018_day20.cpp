#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>


void ParseRegex(std::string& regex)
{
    static int groupMaxLength[1000] = { 0 };
    static int fragmentLength[1000] = { 0 };

    int groupLevel = 0;

    for (int pos = 0; pos < regex.size(); pos++)
    {
        switch (regex[pos])
        {
        case '(':
            groupMaxLength[groupLevel + 1] = 0;
            fragmentLength[groupLevel + 1] = 0;
            ++groupLevel;
            break;
        case ')':
            if (groupMaxLength[groupLevel] < fragmentLength[groupLevel])
            {
                groupMaxLength[groupLevel] = fragmentLength[groupLevel];
            }
            fragmentLength[groupLevel - 1] += groupMaxLength[groupLevel];
            --groupLevel;
            break;
        case '|':
            if (groupMaxLength[groupLevel] < fragmentLength[groupLevel])
            {
                groupMaxLength[groupLevel] = fragmentLength[groupLevel];
            }
            fragmentLength[groupLevel] = 0;
            break;
        case 'N':
        case 'E':
        case 'S':
        case 'W':
            fragmentLength[groupLevel]++;
            break;
        case '^':
            break;
        case '$':
            if (groupMaxLength[groupLevel] < fragmentLength[groupLevel])
            {
                groupMaxLength[groupLevel] = fragmentLength[groupLevel];
            }
            break;
        }
    }
    std::cout << "Max path length is " << groupMaxLength[groupLevel] << "\n";
}

using DeadEnd = std::pair<int, std::string>;
using DeadEndSequenceVec = std::vector<DeadEnd>;

struct RegexFragment
{
    std::string regexFragment;
    std::vector<RegexFragment*> children;

    RegexFragment* proxyOf = nullptr;
    int deadEndIndex = -1;
    bool dirty = false;
};

inline bool IsLeaf(RegexFragment* node)
{
    return node != nullptr && node->proxyOf == nullptr && node->children.size() == 0;
}

void AddAsChildToAllLeaves(RegexFragment* node, RegexFragment* nodeToAdd)
{
    if (node == nullptr || node->dirty)
        return;

    if (IsLeaf(node))
    {
        node->children.push_back(new RegexFragment);
        node->children.back()->proxyOf = nodeToAdd;
        node->dirty = true;
    }
    else
    {
        if (node->proxyOf == nullptr)
        {
            for (auto child : node->children)
                AddAsChildToAllLeaves(child, nodeToAdd);
        }
        else
        {
            AddAsChildToAllLeaves(node->proxyOf, nodeToAdd);
        }
    }
}

void CleanTree(RegexFragment* node)
{
    if (node == nullptr)
        return;

    node->dirty = false;

    if (node->proxyOf == nullptr)
    {
        for (auto child : node->children)
            CleanTree(child);
    }
    else
    {
        //if (node->proxyOf->proxyOf == nullptr
        //    && node->proxyOf->children.size() == 1 && node->proxyOf->regexFragment.size() == 0
        //    && node->proxyOf->children[0]->proxyOf != nullptr)
        //{
        //    node->proxyOf = node->proxyOf->children[0]->proxyOf;
        //}
        CleanTree(node->proxyOf);
    }
}

bool EraseAllSubStr(std::string & str, const std::string & toErase)
{
    size_t pos = std::string::npos;
    bool found = false;
    while ((pos = str.find(toErase)) != std::string::npos)
    {
        str.erase(pos, toErase.length());
        found = true;
    }
    return found;
}

RegexFragment* ParseRegexTree(std::string& regex, const DeadEndSequenceVec& deadEnds)
{
    int groupLevel = 0;
    int currentFragmentStart = 0;
    RegexFragment* root = new RegexFragment;

    RegexFragment* groupRoot[1000];
    RegexFragment* branchRoot[1000];
    RegexFragment* branchCurrent[1000];
    int deadEndIndex = 0;

    for (int pos = 0; pos < regex.size(); pos++)
    {
        if (deadEnds.size() > deadEndIndex && pos == deadEnds[deadEndIndex].first)
        {
            branchCurrent[groupLevel]->deadEndIndex = deadEndIndex;
            deadEndIndex++;
        }

        switch (regex[pos])
        {
        case '(':
            ++groupLevel;

            groupRoot[groupLevel] = new RegexFragment;

            AddAsChildToAllLeaves(branchRoot[groupLevel - 1], groupRoot[groupLevel]);
            CleanTree(branchRoot[groupLevel - 1]);
            branchRoot[groupLevel] = new RegexFragment;
            groupRoot[groupLevel]->children.push_back(branchRoot[groupLevel]);
            branchCurrent[groupLevel] = branchRoot[groupLevel];

            break;
        case ')':
        {
            --groupLevel;
            auto* newFragment = new RegexFragment;
            AddAsChildToAllLeaves(branchRoot[groupLevel], newFragment);
            CleanTree(branchRoot[groupLevel]);
            branchCurrent[groupLevel] = newFragment;
            break;
        }
        case '|':
        {
            branchRoot[groupLevel] = new RegexFragment;
            groupRoot[groupLevel]->children.push_back(branchRoot[groupLevel]);
            branchCurrent[groupLevel] = branchRoot[groupLevel];
            break;
        }

        case 'N':
        case 'E':
        case 'S':
        case 'W':
            branchCurrent[groupLevel]->regexFragment.push_back(regex[pos]);
            break;
        case '^':
            groupRoot[groupLevel] = root;
            branchRoot[groupLevel] = new RegexFragment;
            groupRoot[groupLevel]->children.push_back(branchRoot[groupLevel]);
            branchCurrent[groupLevel] = branchRoot[groupLevel];
            break;
        case '$':

            break;
        }
    }

    return root;
}

void NumRooms(RegexFragment* node, int minLength, int length, int & count, int level, const DeadEndSequenceVec& deadEnds)
{
    if (node)
    {
        if (node->proxyOf == nullptr)
        {
            std::string& regex = node->regexFragment;
            length += regex.size();

            if (node->deadEndIndex != -1 && length + deadEnds[node->deadEndIndex].second.size() >= minLength)
            {
                std::string padding(level, ' ');
                std::cout << padding << regex << " " << (length - minLength >= regex.size() ? regex.size() : length - minLength) + deadEnds[node->deadEndIndex].second.size() << "\n";
                count += (length - minLength >= regex.size() ? regex.size() : length - minLength) + deadEnds[node->deadEndIndex].second.size();
            }
            else if (length >= minLength && regex.size() > 0 )
            {
                std::string padding (level, ' ');
                std::cout << padding << regex << " " << (length - minLength >= regex.size() ? regex.size() : length - minLength) << "\n";
                count += (length - minLength >= regex.size() ? regex.size() : length - minLength);
            }



            auto& children = node->children;

            if (children.size() > 0)
            {
                for (int i = 0; i < children.size(); i++)
                {
                    NumRooms(children[i], minLength, length, count, level + 1, deadEnds);
                }
            }
        }
        else // has a proxy
        {
            NumRooms(node->proxyOf, minLength, length, count, level + 1, deadEnds);
        }
    }
}

int main()
{
    std::string regex;
    std::cin >> regex;
    std::cout << regex << "\n";

    DeadEndSequenceVec deadEndSequences;
    bool foundDeadEnd = false;
    do
    {
        foundDeadEnd = false;

        for (int i = 0; i < regex.size(); ++i)
            if (regex[i] == ')' && regex[i - 1] == '|')
            {
                int j = i - 2;
                while (j >= 0 && regex[j] != '(')
                {
                    --j;
                }
                int deadEndRoomCount = (i - j + 1 - 1 /*pipe*/ - 2 /*parens*/) / 2;
                deadEndSequences.push_back(std::pair(j, std::string(regex.begin() + j + 1, regex.begin() + (j + deadEndRoomCount + 1))));

                regex.erase(j, i - j + 1);
                i = j - 1;
                foundDeadEnd = true;
            }

    } while (foundDeadEnd);


    ParseRegex(regex);
    //deadEndSequences.clear();
    auto root = ParseRegexTree(regex, deadEndSequences);
    int count = 0;
    int minDoors = 1000;
    NumRooms(root, minDoors, 0, count, 0, deadEndSequences);

    std::cout << "Number of different paths above " << minDoors << " " << count << "\n";


    return 0;
}
