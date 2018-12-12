#include <cstdint>
#include <iostream>

struct CellBlockPower
{
    int x, y;
    int size;
    int64_t power;
};

CellBlockPower GetMaxPower(int64_t cells[301][301], int size)
{
    CellBlockPower max = { 0, 0, 0, -90000 };

    for (int x = 1; x <= 300 - size; x++)
    {
        int64_t sum = 0;
        for (int i = x; i < x + size; i++)
            for (int j = 1; j < size; j++)
            {
                sum += cells[i][j];
            }

        for (int y = size; y <= 300; y++)
        {
            for (int i = x; i < x + size; i++)
            {
                sum += cells[i][y];
            }
            if (sum > max.power)
            {
                max.power = sum;
                max.x = x;
                max.y = y + 1 - size;
                max.size = size;
            }
            for (int i = x; i < x + size; i++)
            {
                sum -= cells[i][y + 1 - size];
            }
        }
    }
    return max;
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
        CellBlockPower maxCoords = { 0, 0, 0, -90000 };

        for (int size = 1; size <= 300; size++)
        {
            CellBlockPower coords = GetMaxPower(cells, size);
            if ( coords.power > maxCoords.power )
                maxCoords = coords;
        }
        std::cout << maxCoords.x << "," << maxCoords.y << "," << maxCoords.size;
    }
    return 0;
}