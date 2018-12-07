#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Point
{
    int x = 0;
    int y = 0;
};

struct Rectangle
{
    Point p1, p2;
};

using PointVec = std::vector<Point>;
using ProximityGrid = std::vector<std::vector<int>>;

int ManhattanDistance(Point p1, Point p2)
{
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

PointVec ReadInput()
{
    PointVec points;

    while (true)
    {
        char c;
        Point p;
        if (!(std::cin >> p.x))
            break;
        std::cin >> c >> p.y;
        points.push_back(p);
    }

    return points;
}

Rectangle FindGridLimits(PointVec points)
{
    Point upperLeft{ -1,-1 };
    Point lowerRight{ -1,-1 };
    for(auto& p : points)
    {
        if (upperLeft.x > p.x || upperLeft.x == -1)
            upperLeft.x = p.x;
        if (upperLeft.y > p.y || upperLeft.y == -1)
            upperLeft.y = p.y;
        if (lowerRight.x < p.x || lowerRight.x == -1)
            lowerRight.x = p.x;
        if (lowerRight.y < p.y || lowerRight.y == -1)
            lowerRight.y = p.y;
    }

    return { upperLeft, lowerRight };
}

// Marks a grid area with -1 (practically excluding a pointId from further being considered)
void ExcludeArea(ProximityGrid& grid, int pointId)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == pointId)
            {
                grid[i][j] = -1;
            }
        }
    }
}

size_t Count(ProximityGrid& grid, int value)
{
    size_t counter = 0;

    for (auto& gridRow : grid)
    {
        counter += std::count(gridRow.begin(), gridRow.end(), value);
    }

    return counter;
}

size_t LargestNotInfiniteProximityArea(PointVec& points, const Rectangle& proximityGridLimits)
{
    auto[upperLeft, lowerRight] = proximityGridLimits;

    int m = lowerRight.x - upperLeft.x + 1;
    int n = lowerRight.y - upperLeft.y + 1;

    std::vector<std::vector<int>> grid(m);
    for (int i = 0; i < m; i++)
    {
        grid[i].resize(n);
    }

    for (int x = upperLeft.x; x <= lowerRight.x; x++)
    {
        for (int y = upperLeft.y; y <= lowerRight.y; y++)
        {
            Point p{ x, y };
            std::vector<Point> mds;
            for (int i = 0; i < points.size(); i++)
            {
                mds.push_back(Point{ ManhattanDistance(p, points[i]), i });
            }
            std::sort(mds.begin(), mds.end(), [](Point a, Point b) {return a.x < b.x; });

            int gridX = x - upperLeft.x;
            int gridY = y - upperLeft.y;
            assert(gridX >= 0 && gridX < grid.size());
            assert(gridY >= 0 && gridY < grid[gridX].size());

            if (mds.size() > 1 && mds[0].x == mds[1].x)
            {
                grid[gridX][gridY] = -1;
            }
            else
            {
                grid[gridX][gridY] = mds[0].y;
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        if (grid[i][0] != -1)
        {
            ExcludeArea(grid, grid[i][0]);
        }
        if (grid[i][n - 1] != -1)
        {
            ExcludeArea(grid, grid[i][n - 1]);
        }
    }
    for (int j = 0; j < n; j++)
    {
        if (grid[0][j] != -1)
        {
            ExcludeArea(grid, grid[0][j]);
        }
        if (grid[m - 1][j] != -1)
        {
            ExcludeArea(grid, grid[m - 1][j]);
        }

    }

    std::vector<size_t> areas;
    for (size_t i = 0; i < points.size(); i++)
    {
        areas.push_back(Count(grid, static_cast<int>(i)));
    }

    return *std::max_element(areas.begin(), areas.end());
}

size_t LargestAreaWithBoundTotalCellDistanceToAllPoints(const PointVec& points, const Rectangle& proximityGridLimits, int distanceBound)
{
    auto[upperLeft, lowerRight] = proximityGridLimits;

    int area = 0;

    for (int x = upperLeft.x; x <= lowerRight.x; x++)
    {
        for (int y = upperLeft.y; y <= lowerRight.y; y++)
        {
            Point p{ x, y };
            int sum = 0;
            bool invalid = false;
            for (int i = 0; i < points.size(); i++)
            {
                sum += ManhattanDistance(p, points[i]);
                if (sum >= distanceBound)
                {
                    invalid = true;
                    continue;
                }
            }

            if (!invalid)
                area++;
        }
    }

    return area;
}

int main()
{
    PointVec points = ReadInput();

    Rectangle proximityGridLimits = FindGridLimits(points);

    std::cout << LargestNotInfiniteProximityArea(points, proximityGridLimits) << '\n';

    std::cout << LargestAreaWithBoundTotalCellDistanceToAllPoints(points, proximityGridLimits, 10000) << '\n';

    return 0;
}