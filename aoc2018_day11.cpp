#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <list>

struct CellCoords
{
    int x, y;
    int size;
};

CellCoords GetMaxPower(int64_t cells[301][301], int size)
{
    int64_t maxPower = -900000;
    int xMax = 0;
    int yMax = 0;
    int sizeMax = 0;

    for (int x = 1; x <= 300 - size; x++)
        for (int y = 1; y <= 300 - size; y++)
        {
            int64_t sum = 0;
            for (int i = x; i < x + size; i++)
            {
                for (int j = y; j < y + size; j++)
                    sum += cells[i][j];
            }
            if (sum > maxPower)
            {
                maxPower = sum;
                xMax = x;
                yMax = y;
                sizeMax = size;
            }
        }
    return { xMax, yMax, sizeMax };
}

void ProcessCellPower(int64_t cells[301][301], int64_t serialNumber)
{
    for (int x = 1; x <= 300; x++)
    {
        for (int y = 1; y <= 300; y++)
        {
            int64_t rackID = x + 10;
            int64_t power = rackID * y;
            power += serialNumber;
            power *= rackID;
            power = (power % 1000) / 100;
            power -= 5;
            cells[x][y] = power;
        }
    }
}

int main()
{
    int64_t cells[301][301];
    int64_t serialNumber = 8;
    std::cin >> serialNumber;

    ProcessCellPower(cells, serialNumber);
    {
        auto maxCoords = GetMaxPower(cells, 3);
        std::cout << maxCoords.x << ',' << maxCoords.y << '\n';
    }
    {
        int64_t maxPower = -900000;
        CellCoords maxCoords = { 0, 0, 0 };

        for (int size = 1; size <= 300; size++)
        {
            CellCoords coords = GetMaxPower(cells, size);
            if (cells[coords.x][coords.y] > maxPower)
                maxCoords = coords;
        }
        std::cout << maxCoords.x << "," << maxCoords.y << "," << maxCoords.size;
    }
    return 0;
}