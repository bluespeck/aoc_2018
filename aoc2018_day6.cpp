#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int md(int x1, int y1, int x2, int y2)
{
    return x2 - x2 + y2 - y1;
}

struct Point
{
    int x = 0;
    int y = 0;
};

using PointVec = std::vector<Point>;

int md(Point p1, Point p2)
{
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

void remove(int** grid, int m, int n, int value)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == value)
            {
                grid[i][j] = -1;
            }
        }
    }
}

int count(int** grid, int m, int n, int value)
{
    int counter = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == value)
            {
                counter++;
            }
        }
    }
    return counter;
}

int main()
{
    std::vector<Point> points;
    Point ul{ -1,-1 }, lr{ -1,-1 };
    while(true)
    {
        char c;
        Point p;
        if (!(std::cin >> p.x))
            break;
        std::cin >> c >> p.y;
        points.push_back(p);

        if (ul.x > p.x || ul.x == -1)
            ul.x = p.x;
        if (ul.y > p.y || ul.y == -1)
            ul.y = p.y;
        if (lr.x < p.x || lr.x == -1)
            lr.x = p.x;
        if (lr.y < p.y || lr.y == -1)
            lr.y = p.y;
    }

    int m = lr.x - ul.x + 1;
    int n = lr.y - ul.y + 1;
    int** grid = new int*[m];
    for (int i = 0; i < m; i++)
    {
        grid[i] = new int[n];
    }

    for (int x = ul.x; x <= lr.x; x++)
    {
        for (int y = ul.y; y <= lr.y; y++)
        {
            Point p{ x, y };
            std::vector<Point> mds;
            for (int i = 0; i < points.size(); i++)
            {
                mds.push_back(Point{ md(p, points[i]), i });
            }
            std::sort(mds.begin(), mds.end(), [](Point a, Point b) {return a.x < b.x; });

            if (mds.size() > 1 && mds[0].x == mds[1].x)
            {
                grid[x - ul.x][y - ul.y] = -1;
            }
            else
            {
                grid[x - ul.x][y - ul.y] = mds[0].y;
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        if (grid[i][0] != -1)
        {
            remove(grid, m, n, grid[i][0]);
        }
        if (grid[i][n - 1] != -1)
        {
            remove(grid, m, n, grid[i][n - 1]);
        }
    }
    for (int j = 0; j < n; j++)
    {
        if (grid[0][j] != -1)
        {
            remove(grid, m, n, grid[0][j]);
        }
        if (grid[m-1][j] != -1)
        {
            remove(grid, m, n, grid[m-1][j]);
        }

    }

    std::vector<int> areas;
    for (int i = 0; i < points.size(); i++)
    {
        areas.push_back(count(grid, m, n, i));
    }

    std::cout << *std::max_element(areas.begin(), areas.end()) << '\n';
    int area = 0;
    for (int x = ul.x; x <= lr.x; x++)
    {
        for (int y = ul.y; y <= lr.y; y++)
        {
            Point p{ x, y };
            int sum = 0;
            bool invalid = false;
            for (int i = 0; i < points.size(); i++)
            {
                sum += md(p, points[i]);
                if (sum >= 10000)
                {
                    invalid = true;
                    continue;
                }
            }

            if (!invalid)
                area++;
        }
    }


    std::cout << area << '\n';
    return 0;
}