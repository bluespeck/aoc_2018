#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <list>

using MarbleCircle = std::list<uint64_t>;
using MarbleCircleIterator = std::list<uint64_t>::iterator;
using ScoreVec = std::vector<uint64_t>;

void IncrementCircularIterator(MarbleCircleIterator& it, size_t count, MarbleCircle& marbleCircle)
{
    for (size_t i = 0; i < count; i++)
    {
        if (it == marbleCircle.end())
        {
            it = marbleCircle.begin();
        }
        ++it;
    }
}

void DecrementCircularIterator(MarbleCircleIterator& it, size_t count, MarbleCircle& marbleCircle)
{
    for (size_t i = 0; i < count; i++)
    {
        if (it == marbleCircle.begin())
        {
            it = marbleCircle.end();
        }
        --it;
    }
}

void PrintTurn(int currentPlayer, const MarbleCircleIterator& currentMarbleIt, const MarbleCircle& circle, const ScoreVec& scores)
{
    std::cout << '[' << currentPlayer + 1 << ' ' << scores[currentPlayer] << ']' << ' ';
    for (auto e : circle)
    {
        std::string marble;

        if (e == *currentMarbleIt)
        {
            marble += '(';
        }
        else
        {
            marble += ' ';
        }
        marble += std::to_string(e);
        if (e == *currentMarbleIt)
        {
            marble += ')';
        }
        else
        {
            marble += ' ';
        }
        std::cout << marble;
    }
    std::cout << '\n';
}

void ReadInput(int& numPlayers, uint64_t& lastMarble)
{
    std::cin >> numPlayers;
    std::cin.ignore(31);
    std::cin >> lastMarble;
}

uint64_t WinningScore(int numPlayers, uint64_t lastMarble)
{
    MarbleCircle circle;
    ScoreVec scores(numPlayers);
    uint64_t marbleToPlace = 0;
    int currentPlayer = -1;

    circle.push_back(marbleToPlace);

    MarbleCircleIterator currentMarbleIt = circle.begin();

    while (marbleToPlace < lastMarble)
    {
        currentPlayer = (currentPlayer + 1) % numPlayers;
        marbleToPlace++;
        if (marbleToPlace % 23 != 0)
        {
            IncrementCircularIterator(currentMarbleIt, 2, circle);
            currentMarbleIt = circle.insert(currentMarbleIt, marbleToPlace);
        }
        else
        {
            scores[currentPlayer] += marbleToPlace;
            DecrementCircularIterator(currentMarbleIt, 7, circle);
            scores[currentPlayer] += *currentMarbleIt;
            currentMarbleIt = circle.erase(currentMarbleIt);
        }
        //PrintTurn(currentPlayer, currentMarbleIt, circle, scores);
    }

    return *std::max_element(scores.begin(), scores.end());
}

int main()
{

    int numPlayers = 9;
    uint64_t lastMarble = 93;
    ReadInput(numPlayers, lastMarble);

    std::cout << WinningScore(numPlayers, lastMarble) << '\n';

    std::cout << WinningScore(numPlayers, lastMarble * 100) << '\n';

    return 0;
}
