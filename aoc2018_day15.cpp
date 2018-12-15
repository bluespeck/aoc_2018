#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using Grid = std::vector<std::string>;

struct Target
{
    int32_t x = 0;
    int32_t y = 0;
    int32_t attack = 3;
    int32_t hp = 200;
    char type;

    Target(int32_t x, int32_t y, char type, int32_t attack = 3, int32_t hp = 200)
        : x{ x }, y{ y }, attack{ attack }, hp{ hp }, type{ type }
    {
    }
};

using TargetVec = std::vector<Target>;

struct Cell
{
    int32_t x;
    int32_t y;
    friend bool operator ==(const Cell& a, const Cell& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    friend bool operator !=(const Cell& a, const Cell& b)
    {
        return !(a == b);
    }
};

using CellVec = std::vector<Cell>;

int32_t deltaX[4] = { -1, 0, 0, 1 };
int32_t deltaY[4] = { 0, -1, 1, 0 };

struct Outcome
{
    int32_t hpSum;
    int32_t rounds;
    bool elvesDied;
};

Grid ReadInput()
{
    Grid grid;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        grid.emplace_back(line.begin(), line.end());
    }

    return grid;
}

TargetVec GatherElvesAndGoblins(const Grid& grid, int32_t elfAttack)
{
    TargetVec targets;
    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t j = 0; j < grid[i].size(); ++j)
        {
            const auto& cell = grid[i][j];
            if (cell == 'E')
            {
                targets.emplace_back(i, j, cell, elfAttack);
            }
            else if(cell == 'G')
            {
                targets.emplace_back(i, j, cell);
            }
        }
    }

    return targets;
}

void SortTargetsReadingOrder(TargetVec& targets)
{
    std::sort(targets.begin(), targets.end(), [](const Target& a, const Target& b)
    {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
}

CellVec GetCellsInRange(const Grid& grid, const TargetVec& targets, const Target& target)
{
    CellVec cellsInRange;

    for (const auto& otherTarget : targets)
    {
        if (target.type != otherTarget.type)
        {
            auto x = otherTarget.x;
            auto y = otherTarget.y;
            for (int i = 0; i < 4; i++)
            {
                if (grid[x + deltaX[i]][y + deltaY[i]] == '.')
                    cellsInRange.push_back({ x + deltaX[i], y + deltaY[i] });
            }
        }
    }

    return cellsInRange;
}

CellVec ShortestPathToACellInRange(const Grid& grid, const Target& source, const CellVec& cellsInRange)
{
    std::queue<Cell> toVisit;
    const int32_t infinity = 65000;
    std::vector<std::vector<int32_t>> visited (grid.size(), std::vector<int32_t>(grid.front().size(), infinity));

    visited[source.x][source.y] = 1;
    toVisit.push({ source.x, source.y });

    while (!toVisit.empty())
    {
        auto currentCell = toVisit.front();
        toVisit.pop();

        int32_t x = currentCell.x;
        int32_t y = currentCell.y;
        for (int i = 0; i < 4; i++)
        {
            int32_t xn = x + deltaX[i];
            int32_t yn = y + deltaY[i];
            if (visited[xn][yn] == infinity && grid[xn][yn] == '.')
            {
                toVisit.push({ xn, yn });
                visited[xn][yn] = visited[x][y] + 1;
            }
        }
    }

    CellVec shortestpath;
    auto it = std::min_element(cellsInRange.begin(), cellsInRange.end(), [&visited](auto& a, auto& b)
    {
        return visited[a.x][a.y] < visited[b.x][b.y]
            || (visited[a.x][a.y] == visited[b.x][b.y] && (a.x < b.x || a.x == b.x && a.y < b.y));
    });

    if(it != cellsInRange.end() && visited[it->x][it->y] != infinity)
    {
        auto cell = *it;
        while (visited[cell.x][cell.y] > 1)
        {
            shortestpath.push_back(cell);
            for (int i = 0; i < 4; i++)
            {
                int32_t xn = cell.x + deltaX[i];
                int32_t yn = cell.y + deltaY[i];
                if (visited[xn][yn] == visited[cell.x][cell.y] - 1)
                {
                    cell.x = xn;
                    cell.y = yn;
                    break;
                }
            }
        }
    }

    std::reverse(shortestpath.begin(), shortestpath.end());

    return shortestpath;
}

bool IsAdversary(char otherType, Target& target)
{
    return (otherType == 'G' || otherType == 'E') && otherType != target.type;
}

bool AnyTargetInRange(const Grid& grid, Target& target)
{
    auto x = target.x;
    auto y = target.y;
    for (int i = 0; i < 4; i++)
    {
        if (IsAdversary(grid[x + deltaX[i]][y + deltaY[i]], target))
        {
            return true;
        }
    }

    return false;
}

ptrdiff_t AttackTargetInRangeOf(Grid& grid, TargetVec& targets, Target& target, bool& elvesDied)
{
    auto itMin = targets.end();
    auto x = target.x;
    auto y = target.y;
    for (int i = 0; i < 4; ++i)
    {
        auto xn = x + deltaX[i];
        auto yn = y + deltaY[i];

        if (IsAdversary(grid[xn][yn], target))
        {
            auto it = std::find_if(targets.begin(), targets.end(), [xn, yn](auto& target) {
                return target.x == xn && target.y == yn;
            });
            assert(it != targets.end());
            if (itMin == targets.end() || it->hp < itMin->hp)
                itMin = it;
        }
    }

    if (itMin != targets.end())
    {
        itMin->hp -= target.attack;
        if (itMin->hp < 0)
        {
            if (itMin->type == 'E')
                elvesDied = true;
            grid[itMin->x][itMin->y] = '.';

            ptrdiff_t diff = std::distance(targets.begin(), itMin);
            targets.erase(itMin);
            return diff;
        }
    }
    return -1;
}

void MoveTarget(Grid& grid, TargetVec& targets, Target& target)
{
    auto cellsInRange = GetCellsInRange(grid, targets, target);
    if (cellsInRange.size() == 0)
        return;
    auto shortestPath = ShortestPathToACellInRange(grid, target, cellsInRange);

    if (shortestPath.size() == 0)
        return;

    grid[target.x][target.y] = '.';
    target.x = shortestPath[0].x;
    target.y = shortestPath[0].y;
    grid[target.x][target.y] = target.type;
}

void ActRound(Grid& grid, TargetVec& targets, bool& elvesDied)
{
    SortTargetsReadingOrder(targets);

    for (int32_t i = 0; i < targets.size(); i++)
    {
        auto& target = targets[i];
        if (!AnyTargetInRange(grid, target))
        {
            MoveTarget(grid, targets, target);
        }

        auto index = AttackTargetInRangeOf(grid, targets, target, elvesDied);
        if (index != -1)
        {
            if (index <= i)
                i--;
        }
    }
}

void PrintGrid(int32_t numRounds, const Grid& grid, const TargetVec& targets)
{
    std::cout << numRounds << "\n";
    auto it = targets.begin();
    for (int i = 0; i < grid.size(); ++i)
    {
        std::cout << grid[i] << ' ';
        while (it != targets.end() && it->x == i)
        {
            std::cout << it->type << "(" << it->hp << ") ";
            it++;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int32_t SumHPs(const TargetVec& targets)
{
    int32_t hpSum = 0;
    for (auto& target : targets)
    {
        hpSum += target.hp;
    }

    return hpSum;
}

Outcome PlayOut(const Grid& initialGrid, int32_t elfAttack = 3)
{
    Grid grid = initialGrid;
    TargetVec targets = GatherElvesAndGoblins(grid, elfAttack);
    bool elvesDied = false;
    int32_t numRounds = 0;
    //PrintGrid(numRounds, grid, targets);
    do
    {
        auto goblinCount = std::count_if(targets.begin(), targets.end(), [](const Target& target) { return target.type == 'G'; });
        auto elfCount = std::count_if(targets.begin(), targets.end(), [](const Target& target) { return target.type == 'E'; });
        if (goblinCount == 0 || elfCount == 0)
            break;

        ActRound(grid, targets, elvesDied);

        numRounds++;

        //PrintGrid(numRounds, grid, targets);

    } while (true);

    Outcome outcome;

    outcome.hpSum = SumHPs(targets);
    outcome.rounds = numRounds - 1;
    outcome.elvesDied = elvesDied;

    return outcome;
}

int main()
{
    Grid grid = ReadInput();

    {
        auto outcome = PlayOut(grid);
        std::cout << outcome.hpSum * outcome.rounds << "\n";
    }

    {
        Outcome goodOutcome;
        int32_t elfAttackMax = 4096;
        int32_t elfAttackMin = 3;
        while(elfAttackMin < elfAttackMax)
        {
            int32_t elfAttack = (elfAttackMax + elfAttackMin) / 2;
            auto outcome = PlayOut(grid, elfAttack);
            if (!outcome.elvesDied)
            {
                goodOutcome = outcome;
                elfAttackMax = elfAttack - 1;
            }
            else
            {
                elfAttackMin = elfAttack + 1;
            }
        }
        std::cout << goodOutcome.hpSum * goodOutcome.rounds << "\n";
    }
    return 0;
}