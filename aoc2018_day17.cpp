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

struct Line
{
    char type;
    int d1;
    int d2_1, d2_2;
};

using LineVec = std::vector<Line>;
using Grid = std::vector<std::vector<char>>;

LineVec ReadInput()
{
    LineVec lines;

    std::string line;
    while (std::getline(std::cin, line))
    {
        std::istringstream in(line);
        Line gridLine;

        gridLine.type = (line[0] == 'y') ? 'h' : 'v';

        in.ignore(2);
        in >> gridLine.d1;
        in.ignore(4);
        in >> gridLine.d2_1;
        in.ignore(2);
        in >> gridLine.d2_2;

        lines.push_back(gridLine);
    }
    return lines;
}

void PrintGrid(Grid& grid)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            std::cout << grid[i][j];
        };
        std::cout << '\n';
    }
    std::cout << '\n';
}


Grid MakeGrid(const LineVec& lines, int& hOffset, int& vMin, int& vMax)
{
    int minX = 5000, maxX = 0;
    int minY = 5000, maxY = 0;

    for (auto line : lines)
    {
        if (line.type == 'h')
        {
            if (line.d2_1 < minX)
                minX = line.d2_1;
            if (line.d2_2 > maxX)
                maxX = line.d2_2;
            if (line.d1 < minY)
                minY = line.d1;
            if (line.d1 > maxY)
                maxY = line.d1;
        }
        else
        {
            if (line.d2_1 < minY)
                minY = line.d2_1;
            if (line.d2_2 > maxY)
                maxY = line.d2_2;
            if (line.d1 < minX)
                minX = line.d1;
            if (line.d1 > maxX)
                maxX = line.d1;
        }
    }

    minX--;
    maxX+=2;
    hOffset = minX;
    vMin = minY;
    vMax = maxY;
    Grid grid;

    grid.resize(maxY + 1);
    for (size_t i = 0; i < grid.size(); i++)
    {
        grid[i].resize(maxX - minX + 1, '.');
    }

    for (auto line : lines)
    {
        if (line.type == 'h')
        {
            for (int i = line.d2_1; i <= line.d2_2; i++)
            {
                grid[line.d1][i - hOffset] = '#';
            }
        }
        else
        {
            for (int i = line.d2_1; i <= line.d2_2; i++)
            {
                grid[i][line.d1 - hOffset] = '#';
            }
        }
    }

    return grid;
}

void SimulateWater(Grid& grid, int hOffset)
{
    int m = grid.size();
    int n = grid[0].size();
    bool waterAdded = true;

    while (waterAdded)
    {
        waterAdded = false;
        for (size_t i = 1; i < m - 1; i++)
        {
            for (size_t j = 1; j < n - 1; j++)
            {
                if (grid[i][j] == '|')
                {
                    if (grid[i + 1][j] == '.')
                    {
                        grid[i + 1][j] = '|';
                        waterAdded = true;
                    }
                    else if (grid[i + 1][j] == '#' || grid[i + 1][j] == '~')
                    {
                        if (grid[i][j - 1] == '.')
                        {
                            grid[i][j - 1] = '|';
                            waterAdded = true;
                        }
                        if (grid[i][j + 1] == '.')
                        {
                            grid[i][j + 1] = '|';
                            waterAdded = true;
                        }

                        bool shouldBeStill = true;
                        {
                            for (int k = j - 1; k > 0 && grid[i][k] != '#' && grid[i][k] != '~'; k--)
                               if (grid[i][k] == '.')
                                    shouldBeStill = false;
                        }

                        if (shouldBeStill)
                        {
                            for (int k = j + 1; k < n && grid[i][k] != '#' && grid[i][k] != '~'; k++)
                                if (grid[i][k] == '.')
                                    shouldBeStill = false;
                        }

                        if (shouldBeStill)
                        {
                            grid[i][j] = '~';
                            waterAdded = true;
                        }
                    }
                }
            }
        }
        //PrintGrid(grid);
    }
}

int CountCells(Grid& grid, size_t firstRow, size_t lastRow, const std::vector<char>& cellValues)
{
    int cellCount = 0;
    for (int i = firstRow; i <= lastRow; i++)
        for (int k = 0; k < grid[i].size(); k++)
        {
            for(auto cellValue : cellValues)
            if (grid[i][k] == cellValue)
                cellCount++;
        }
    return cellCount;
}

int main()
{
    LineVec lines = ReadInput();
    int hOffset = 0;
    int firstRow = 0, lastRow = 0;
    Grid grid = MakeGrid(lines, hOffset, firstRow, lastRow);
    grid[0][500 - hOffset] = '+';
    grid[1][500 - hOffset] = '|';
    SimulateWater(grid, hOffset);
    //PrintGrid(grid);

    std::cout << CountCells(grid, firstRow, lastRow, { '~', '|' }) << '\n';
    std::cout << CountCells(grid, firstRow, lastRow, { '~' }) << '\n';

    return 0;
}




