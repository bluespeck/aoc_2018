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

struct Point
{
    int x;
    int y;
};

using PointVec = std::vector<Point>;

void ReadInput(PointVec& points, PointVec&velocities)
{
    while (std::cin)
    {
        Point point, velocity;
        std::cin.ignore(10);
        std::cin >> point.x;
        std::cin.ignore(1);
        std::cin >> point.y;
        std::cin.ignore(12);
        std::cin >> velocity.x;
        std::cin.ignore(1);
        std::cin >> velocity.y;
        std::cin.ignore(2);

        points.push_back(point);
        velocities.push_back(velocity);
    }
}

int PrintMessage(PointVec& points, const PointVec& velocities)
{
    int timeSkip = std::abs(points[0].x / velocities[0].x);

    for (int i = 0; i < points.size(); i++)
    {
        points[i].x += timeSkip * velocities[i].x;
        points[i].y += timeSkip * velocities[i].y;
    }

    for (int t = 0; t < 100; t++)
    {
        int yMin = 1000;
        int yMax = -1000;
        int xMin = 1000;
        int xMax = -1000;
        for (int i = 0; i < points.size(); i++)
        {
            points[i].x += velocities[i].x;
            points[i].y += velocities[i].y;
            if (points[i].y > yMax)
                yMax = points[i].y;
            if (points[i].y < yMin)
                yMin = points[i].y;
            if (points[i].x > xMax)
                xMax = points[i].x;
            if (points[i].x < xMin)
                xMin = points[i].x;
        }

        int width = xMax - xMin + 1;
        int height = yMax - yMin + 1;

        if (height <= 10)
        {
            std::string *message = new std::string[height];
            for (int i = 0; i < height; i++)
            {
                message[i].resize(width, ' ');
            }

            for (int i = 0; i < points.size(); i++)
            {
                message[points[i].y - yMin][points[i].x - xMin] = 'X';
            }

            for (int i = 0; i < height; i++)
            {
                std::cout << message[i] << '\n';
            }
            return timeSkip + t + 1;
        }
    }
    return 0;
}

int main()
{
    std::vector<Point> points;
    std::vector<Point> velocities;

    ReadInput(points, velocities);

    int timeToMessage = PrintMessage(points, velocities);

    std::cout << timeToMessage << '\n';

    return 0;
}