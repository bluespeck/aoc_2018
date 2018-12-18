#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
const int size = 52;

using Grid = std::array<std::array<char, size>, size>;

int dx[8] = { -1, -1, -1, 0,  0,  1, 1, 1 };
int dy[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };

Grid ReadInput()
{
    Grid grid;
    int i = 1;

    for (int j = 0; j < grid[0].size(); j++)
        grid[0][j] = 0;
    for (int j = 0; j < grid[0].size(); j++)
        grid[grid[0].size() - 1][j] = 0;

    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        grid[i][0] = 0;
        for (int j = 0; j < line.size(); j++)
        {
            grid[i][j + 1] = line[j];
        }
        grid[i][grid[i].size() - 1] = 0;
        i++;
    }

    return grid;
}

int CountAdjacentAcres(Grid&grid, int x, int y, char acre)
{
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (grid[nx][ny] == acre)
        {
            count++;
        }
    }
    return count;
}

void NextGrid(Grid& grid)
{
    std::array<char, size> newPrevLine;
    std::array<char, size> newLine;

    for (int i = 1; i < grid.size() - 1; i++)
    {
        newLine = grid[i];
        for (int j = 1; j < grid[i].size() - 1; j++)
        {
            switch (grid[i][j])
            {
            case '.':
                if (CountAdjacentAcres(grid, i, j, '|') >= 3)
                    newLine[j] = '|';
                break;
            case '|':
                if (CountAdjacentAcres(grid, i, j, '#') >= 3)
                    newLine[j] = '#';
                break;
            case '#':
                if (CountAdjacentAcres(grid, i, j, '#') < 1 || CountAdjacentAcres(grid, i, j, '|') < 1)
                    newLine[j] = '.';
                break;
            }
        }
        if (i >= 2)
            grid[i - 1] = newPrevLine;
        newPrevLine = newLine;
    }
    grid[grid.size() - 2] = newLine;
}

int32_t ComputeResourceValue(const Grid& grid)
{
    int32_t trees = 0, lumberyards = 0;
    for (int i = 1; i < grid.size() - 1; i++)
    {
        for (int j = 1; j < grid[i].size() - 1; j++)
        {
            if (grid[i][j] == '|')
                trees++;
            else if (grid[i][j] == '#')
                lumberyards++;
        }
    }

    return trees * lumberyards;
}

void PrintGrid(const Grid& grid)
{
    for (int i = 1; i < grid.size() - 1; i++)
    {
        for (int j = 1; j < grid[i].size() - 1; j++)
        {
            std::cout << grid[i][j];
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void FindPeriodAndStartIndex(std::vector<int64_t>& values, size_t& period, size_t& startIndex)
{
    period = 0;
    startIndex = 0;
    size_t maxPeriod = values.size() / 2;
    for (size_t tryPeriod = 2; tryPeriod <= maxPeriod; tryPeriod++)
    {
        for (size_t tryStartIndex = 0; tryStartIndex < values.size() - tryPeriod; tryStartIndex++)
        {
            bool periodFound = true;
            for (size_t i = 0; i < tryPeriod; i++)
            {
                if (values[tryStartIndex + i] != values[tryStartIndex + i + tryPeriod])
                {
                    periodFound = false;
                    break;
                }
            }

            if (periodFound)
            {
                period = tryPeriod;
                startIndex = tryStartIndex;
                break;
            }
        }
        if (period != 0)
            break;
    }
}

int64_t ComputeResourceValueAfterTimePeriod(Grid grid, size_t minutes)
{
    if (minutes < 2000)
    {
        for (int i = 0; i < 10; i++)
        {
            NextGrid(grid);
        }
        return ComputeResourceValue(grid);
    }
    else
    {
        size_t sampleSize = 2000; // sample size in which to test if the grid has become periodic

        std::vector<int64_t> sampleResourceValues;

        for (size_t i = 0; i < sampleSize; i++)
        {
            NextGrid(grid);
            sampleResourceValues.push_back(ComputeResourceValue(grid));
        }

        size_t startSampleIndex, period;
        FindPeriodAndStartIndex(sampleResourceValues, period, startSampleIndex);
        std::cout << period << " " << startSampleIndex << "\n";
        size_t index = startSampleIndex + (1'000'000'000 - (startSampleIndex + 1)) % period;
        return sampleResourceValues[index];
    }
}

int main()
{
    Grid grid = ReadInput();

    std::cout << ComputeResourceValueAfterTimePeriod(grid, 10) << "\n";
    std::cout << ComputeResourceValueAfterTimePeriod(grid, 1'000'000'000) << "\n";

    return 0;
}
