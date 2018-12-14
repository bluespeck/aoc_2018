#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <set>
#include <string>
#include <vector>

void IncrementIt(std::list<int>::iterator& it, uint64_t amount, std::list<int>& recipes)
{
    for (uint64_t i = 0; i < amount; ++i)
    {
        ++it;
        if (it == recipes.end())
            it = recipes.begin();

    }
}

int64_t CountBeforeSequence(std::list<int>& recipes, std::vector<int> sequence)
{
    auto it = recipes.end();
    while (it != recipes.begin() && std::distance(it, recipes.end()) <= 2)
    {
        --it;
        if (*it == sequence[sequence.size() - 1])
        {
            int seqIndex = sequence.size() - 2;
            auto bIt = it;
            --bIt;
            for (; bIt != recipes.begin() && seqIndex >= 0; --bIt, --seqIndex)
            {
                if (*bIt != sequence[seqIndex])
                    break;
            }
            if (seqIndex == -1)
            {
                return std::distance(recipes.begin(), bIt);
            }
        }
    }
    return -1;
}

void CheckFirstProblemSolved(std::list<int>& recipes, uint64_t maxRecipeCount, bool& firstProblemSolved)
{
    if (!firstProblemSolved && recipes.size() >= static_cast<size_t>(maxRecipeCount + 10))
    {
        auto it = recipes.begin();
        IncrementIt(it, maxRecipeCount, recipes);
        for (uint64_t i = maxRecipeCount + 1; i <= maxRecipeCount + 10; i++)
        {
            std::cout << *it;
            ++it;
        }
        std::cout << '\n';
        firstProblemSolved = true;
    }
}

void CheckSecondProblemSolved(std::list<int>& recipes, const std::vector<int>& sequence, int& seqIndex, bool& secondProblemSolved)
{
    if (!secondProblemSolved && recipes.back() == sequence[seqIndex])
    {
        ++seqIndex;
        if (seqIndex == sequence.size())
        {
            std::cout << recipes.size() - sequence.size();
            secondProblemSolved = true;
        }
    }
    else
    {
        seqIndex = 0;
    }
}

void ReadInput(uint64_t& maxRecipeCount, std::vector<int>& sequence)
{
    std::cin >> maxRecipeCount;

    uint64_t temp = maxRecipeCount;
    while (temp)
    {
        sequence.push_back(temp % 10);
        temp /= 10;
    }
    std::reverse(sequence.begin(), sequence.end());

}

int main()
{
    uint64_t maxRecipeCount;
    std::vector<int> sequence;
    ReadInput(maxRecipeCount, sequence);

    std::list<int> recipes;

    recipes.push_back(3);
    recipes.push_back(7);
    auto it1 = recipes.begin();
    auto it2 = recipes.begin(); it2++;

    bool firstProblemSolved = false;
    bool secondProblemSolved = false;
    int seqIndex = 0;

    while(!firstProblemSolved || !secondProblemSolved)
    {
        CheckFirstProblemSolved(recipes, maxRecipeCount, firstProblemSolved);

        uint64_t sum = *it1 + *it2;
        if (sum >= 10)
        {
            int newRecipeScore = sum / 10;
            recipes.push_back(newRecipeScore);
            CheckSecondProblemSolved(recipes, sequence, seqIndex, secondProblemSolved);
        }
        int newRecipeScore = sum % 10;
        recipes.push_back(newRecipeScore);
        CheckSecondProblemSolved(recipes, sequence, seqIndex, secondProblemSolved);

        IncrementIt(it1, 1 + *it1, recipes);
        IncrementIt(it2, 1 + *it2, recipes);
    }

    return 0;
}