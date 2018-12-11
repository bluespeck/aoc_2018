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

int main()
{
    int64_t cells[301][301];
    int64_t sn = 8;
    std::cin >> sn;

    for (int x = 1; x <= 300; x++)
    {
        for (int y = 1; y <= 300; y++)
        {
            int64_t rackID = x + 10;
            int64_t power = rackID * y;
            power += sn;
            power *= rackID;
            power = (power % 1000) / 100;
            power -= 5;
            cells[x][y] = power;
        }
    }

    int64_t maxPower = -900000;
    int xMax = 0;
    int yMax = 0;
    int sizeMax = 0;

    for (int size = 1; size <= 300; size++)
    {
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
    }

    std::cout << xMax << "," << yMax << "," << sizeMax;
    return 0;
}