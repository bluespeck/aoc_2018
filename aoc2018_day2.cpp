#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using CountsArray = std::array<int, 26>;
using IdArray = std::vector<std::string>;

IdArray ReadInput()
{
    IdArray idArray;
    char id[100];
    while (std::cin.getline(id, 100))
    {
        idArray.push_back(id);
    }

    return idArray;
}

std::array<int, 26> GetCharCounts(const std::string&id)
{
    std::array<int, 26> counts;
    std::fill(counts.begin(), counts.end(), 0);

    for (int i = 0; i < id.size(); i++)
    {
        counts[id[i] - 'a'] ++;
    }

    return counts;
}

int HasExactlyTwo(const std::string& id, const CountsArray& counts)
{
    for (auto count : counts)
    {
        if (count == 2)
            return 1;
    }

    return 0;
}

int HasExactlyThree(const std::string& id, const CountsArray& counts)
{
    for (auto count : counts)
    {
        if (count == 3)
            return 1;
    }

    return 0;
}

int CheckSum(const IdArray& idArray)
{
    int numTwos = 0;
    int numThrees = 0;
    for (auto& id : idArray)
    {
        auto counts = GetCharCounts(id);
        numTwos += HasExactlyTwo(id, counts);
        numThrees += HasExactlyThree(id, counts);
    }

    return numTwos * numThrees;
}

bool AreCorrectIds(const std::string& id1, const std::string& id2)
{
    for (int i = 0; i < id1.size(); i++)
    {
        if (id1[i] != id2[i])
        {
            for (int j = i + 1; j < id1.size(); j++)
            {
                if (id1[j] != id2[j])
                    return false;
            }

            return true;
        }
    }

    return false;
}

void PrintIdenticalCorrespondingLetters(const std::string& id1, const std::string& id2)
{
    for (int i = 0; i < id1.size(); i++)
    {
        if (id1[i] == id2[i])
        {
            std::cout << id1[i];
        }
    }
    std::cout << "\n";
}

void PrintCorrectIDs(const IdArray& idArray)
{
    for (int i = 0; i < idArray.size() -1; i++)
    {
        for (int j = i + 1; j < idArray.size(); j++)
        {
            if (AreCorrectIds(idArray[i], idArray[j]))
            {
                PrintIdenticalCorrespondingLetters(idArray[i], idArray[j]);
                return;
            }
        }
    }
}

int main()
{
    auto idArray = ReadInput();

    std::cout << CheckSum(idArray) << '\n';
    PrintCorrectIDs(idArray);

    return 0;
}