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

int CountAcres(Grid&grid, int x, int y, char acre)
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
                if (CountAcres(grid, i, j, '|') >= 3)
                    newLine[j] = '|';
                break;
            case '|':
                if (CountAcres(grid, i, j, '#') >= 3)
                    newLine[j] = '#';
                break;
            case '#':
                if (CountAcres(grid, i, j, '#') < 1 || CountAcres(grid, i, j, '|') < 1)
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

int main()
{
    Grid grid = ReadInput();

    int minutes = 1000;

    for (int64_t i = 0; i < minutes; i++)
        NextGrid(grid);

    int trees = 0, lumberyards = 0;
    for (int i = 1; i < grid.size() -1 ; i++)
    {
        for (int j = 1; j < grid[i].size() - 1; j++)
        {
            std::cout << grid[i][j];
            if (grid[i][j] == '|')
                trees++;
            else if (grid[i][j] == '#')
                lumberyards++;
        }
        std::cout << "\n";
    }

    std::cout << trees * lumberyards;

    return 0;
}
