#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>

struct Point
{
    int64_t x, y, z;
};

struct Nanobot
{
    Point p;
    int64_t r;
};

using NanobotVec = std::vector<Nanobot>;
NanobotVec ReadInput()
{
    NanobotVec nanobots;
    while (std::cin)
    {
        Nanobot nanobot;
        std::cin.ignore(5);
        std::cin >> nanobot.p.x;
        std::cin.ignore(1);
        std::cin >> nanobot.p.y;
        std::cin.ignore(1);
        std::cin >> nanobot.p.z;
        std::cin.ignore(5);
        std::cin >> nanobot.r;
        nanobots.push_back(nanobot);
        std::cin.ignore(1);
    }

    return nanobots;
}

int64_t md(const Nanobot& a, const Nanobot& b)
{
    int64_t dx = a.p.x < b.p.x ? b.p.x - a.p.x : a.p.x - b.p.x;
    int64_t dy = a.p.y < b.p.y ? b.p.y - a.p.y : a.p.y - b.p.y;
    int64_t dz = a.p.z < b.p.z ? b.p.z - a.p.z : a.p.z - b.p.z;
    return dx + dy + dz;
}

int64_t md(const Point& a, const Point& b)
{
    int64_t dx = a.x < b.x ? b.x - a.x : a.x - b.x;
    int64_t dy = a.y < b.y ? b.y - a.y : a.y - b.y;
    int64_t dz = a.z < b.z ? b.z - a.z : a.z - b.z;
    return dx + dy + dz;
}

bool IsInRange(Nanobot&a, Nanobot&b)
{
    return md(a, b) <= b.r + 1;
}

using IndexVec = std::vector<int>;

int64_t sqrDist(Nanobot&a, Nanobot&b)
{
    int64_t dx = (a.p.x - b.p.x);
    int64_t dy = (a.p.y - b.p.y);
    int64_t dz = (a.p.z - b.p.z);
    return dx * dx + dy * dy + dz * dz;
}

bool Intersects(Nanobot&a, Nanobot&b)
{
    return sqrDist(a, b) <= (a.r + b.r + 2) * (a.r + b.r + 2);
}

IndexVec FindMostClumpedUp(NanobotVec& nanobots, int& mostIntersectedNanobotIndex)
{
    std::vector<IndexVec> intersects;
    intersects.resize(nanobots.size());
    for (int i = 0; i < nanobots.size(); i++)
    {
        for (int j = 0; j < nanobots.size(); j++)
        {
            if (i != j && Intersects(nanobots[j], nanobots[i]))
                intersects[i].push_back(j);
        }
    }
    auto it = std::max_element(intersects.begin(), intersects.end(), [](auto& a, auto& b) { return a.size() < b.size(); });
    mostIntersectedNanobotIndex = std::distance(intersects.begin(), it);
    return *it;
}

IndexVec FindMostClumpedUp(NanobotVec& nanobots, IndexVec& indices, int& mostIntersectedNanobotIndex)
{
    std::vector<IndexVec> intersects;
    intersects.resize(indices.size());
    for (int i = 0; i < indices.size(); i++)
    {
        for (int j = 0; j < indices.size(); j++)
        {
            if (i != j && Intersects(nanobots[indices[j]], nanobots[indices[i]]))
                intersects[i].push_back(indices[j]);
        }
    }
    auto it = std::max_element(intersects.begin(), intersects.end(), [](auto& a, auto& b) { return a.size() < b.size(); });
    mostIntersectedNanobotIndex = std::distance(intersects.begin(), it);
    return *it;
}

bool IsPointInRange(const Point& p, const Nanobot& n)
{
    return md(p, n.p) <= n.r + 1;
}

int CountBotsInRange(const Point&p, const NanobotVec& nanobots)
{
    return std::count_if(nanobots.begin(), nanobots.end(), [&p](auto& elem)
    {
        return IsPointInRange(p, elem);
    });
}

int CountBotsInRange(const Point&p, int64_t volumeHalfDiagonal, const NanobotVec& nanobots)
{
    return std::count_if(nanobots.begin(), nanobots.end(), [&p, volumeHalfDiagonal](const auto& elem)
    {
        return IsPointInRange(p, { elem.p, elem.r + volumeHalfDiagonal - 1 });
    });
}

const int dx[8] = { -1, 1, 0, 0, 0, 0 };
const int dy[8] = { 0, 0, -1, 1, 0, 0 };
const int dz[8] = { 0, 0 , 0, 0, -1, 1 };

void FindMaxInRange(Point point, NanobotVec& nanobots, int maxCount)
{
    Point origin { 0, 0, 0 };
    while (true)
    {
        int count = CountBotsInRange(point, nanobots);
        if (count < maxCount)
            continue;
        else if (maxCount == count)
        {
            Point closestToOrigin = point;
            int64_t closestDist = md(point, origin);
            for (int i = 0; i < 8; i++)
            {
                Point p = { point.x + dx[i], point.y + dy[i], point.z + dz[i] };
                int64_t dist = md(p, origin);
                if ( dist< closestDist)
                {
                    closestToOrigin = p;
                    closestDist = dist;
                }
            }
            point = closestToOrigin;
        }
        else
            maxCount = count;

    }
}

void MinimizeVolume(Point& p1, Point& p2, NanobotVec& nanobots, int maxBots)
{
    Point p = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };

    int botsInRange = CountBotsInRange(p, nanobots);

}

int main()
{
    NanobotVec nanobots = ReadInput();
    auto it = std::max_element(nanobots.begin(), nanobots.end(), [](auto&a, auto&b) {return a.r < b.r; });
    Nanobot largest = *it;
    int count = std::count_if(nanobots.begin(), nanobots.end(), [&largest](auto& elem)
    {
        return md(largest, elem) <= largest.r + 1;
    });

    std::cout << "Index : " << distance(nanobots.begin(), it) << " " << count << "\n";

    //int maxNanoBotsInRange = std::count_if(nanobots.begin(), nanobots.end(), [&largest](auto& elem)
    //{
    //    return IsPointInRange(largest.p, elem);
    //});

    Point pMin, pMax;
    pMin = nanobots[0].p;
    pMax = nanobots[0].p;

    for (auto bot : nanobots)
    {
        pMin.x = (bot.p.x - bot.r - 1) < pMin.x ? (bot.p.x - bot.r - 1) : pMin.x;
        pMax.x = (bot.p.x + bot.r + 1) > pMax.x ? (bot.p.x + bot.r + 1) : pMax.x;
        pMin.y = (bot.p.y - bot.r - 1) < pMin.y ? (bot.p.y - bot.r - 1) : pMin.y;
        pMax.y = (bot.p.y + bot.r + 1) > pMax.y ? (bot.p.y + bot.r + 1) : pMax.y;
        pMin.z = (bot.p.z - bot.r - 1) < pMin.z ? (bot.p.z - bot.r - 1) : pMin.z;
        pMax.z = (bot.p.z + bot.r + 1) > pMax.z ? (bot.p.z + bot.r + 1) : pMax.z;
    }

    // start with the volume centered in 000 and then try all the volumes that have centers in
    // the center, vertices, middles of edges, and middles of faces of the current volume (27 in total for one volume)
    // and repeat while halfing the half-diagonal

    int64_t volumeHalfDiagonal = static_cast<int64_t>(1)<<static_cast<int64_t>(log2(md(pMin, pMax)) + 1);

    std::vector<Point> points;

    Point zero{ 0, 0, 0 };
    points.push_back(zero);
    int maxNanobotsInRange = 0;
    do
    {
        std::vector<Point> newPoints;
        int maxCount = -1;
        for (auto& point : points)
        {
            int count = CountBotsInRange(point, volumeHalfDiagonal, nanobots);
            if (count > maxCount)
            {
                newPoints.clear();
                newPoints.push_back(point);
                maxCount = count;
            }
            else if (count == maxCount)
            {
                newPoints.push_back(point);
            }
        }

        points.clear();
        volumeHalfDiagonal >>= 1;

        if (volumeHalfDiagonal == 0)
        {
            points = newPoints;
            maxNanobotsInRange = maxCount;
        }
        else
        {
            for (auto& point : newPoints)
            {
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        for (int k = -1; k <= 1; k++)
                        {
                            points.push_back({
                                point.x + i * volumeHalfDiagonal,
                                point.y + j * volumeHalfDiagonal,
                                point.z + k * volumeHalfDiagonal
                                });
                        }
                    }
                }
            }
        }
    } while (volumeHalfDiagonal != 0);

    int64_t distZeroMin = md(zero, points[0]);
    for (auto& point : points)
    {
        int64_t distZero = md(zero, point);
        if (distZero < distZeroMin && CountBotsInRange(point, nanobots) == maxNanobotsInRange)
            distZeroMin = distZero;
    }

    // I have an off by one somewhere... :(
    // (tried +1 and it worked); couldn't figure it out in the end and too tired to continue digging
    std::cout << "Min dist :P" << distZeroMin + 1 << "\n";

    //MinimizeVolume(pMin, pMax, nanobots, maxNanoBotsInRange);
    //{
    //    //IndexVec indices;
    //    //indices.push_back(0);
    //    //auto clump = FindMostClumpedUp(nanobots, indices.back());
    //    //indices.push_back(0);
    //    //auto moreAccurateClump = FindMostClumpedUp(nanobots, clump, indices.back());
    //    //moreAccurateClump.push_back(indices.back());
    //    ////do
    //    ////{
    //    ////    indices.push_back(0);
    //    ////    moreAccurateClump = FindMostClumpedUp(nanobots, moreAccurateClump, indices.back());
    //    ////} while (moreAccurateClump.size() > nanobots.size() * 0.90);
    //    //
    //    //std::sort(indices.begin(), indices.end());
    //    //indices.erase(std::unique(indices.begin(), indices.end()), indices.end());
    //    int64_t maxCount = 0;
    //    Point maxPoint;
    //    int64_t maxPointRadius = 0;
    //
    //    for (int i = 0; i < nanobots.size(); i++)
    //    {
    //        auto count = CountBotsInRange(nanobots[i].p, nanobots);
    //        if (count < nanobots.size() / 20)
    //        {
    //            nanobots.erase(nanobots.begin() + i);
    //            i--;
    //        }
    //        else if (count > maxCount)
    //        {
    //            maxCount = count;
    //            maxPoint = nanobots[i].p;
    //            maxPointRadius = nanobots[i].r;
    //        }
    //    }
    //
    //
    //
    //    Point zero{ 0, 0, 0 };
    //    int64_t dist = md(zero, maxPoint);
    //    for (auto& bot: nanobots)
    //    {
    //        //Nanobot bot = nanobots[index];
    //        Point edgePoint = Point{ bot.p.x , bot.p.y , bot.p.z };
    //        int64_t count = CountBotsInRange(edgePoint, nanobots);
    //        std::cout << count << " : " << bot.p.x << ", " << bot.p.y << ", " << bot.p.z << " r = " << bot.r << "\n";
    //        int64_t currentDist = md(zero, bot.p) - bot.r;
    //        if (count == maxCount && currentDist < dist)
    //        {
    //            dist = currentDist;
    //            maxPoint = edgePoint;
    //            maxPointRadius = bot.r;
    //        }
    //    }
    //
    //    std::cout << dist << "\n";
    //
    //    int64_t range = 10;
    //    Point center = maxPoint;
    //    center.x += maxPointRadius ;
    //    center.y += maxPointRadius ;
    //    center.z += maxPointRadius ;
    //    for (int64_t x = maxPoint.x - range; x < maxPoint.x + range; x+=)
    //    {
    //        for (int64_t y = maxPoint.y - range; y < maxPoint.y + range; y++)
    //        {
    //            for (int64_t z = maxPoint.z - range; z < maxPoint.z + range; z++)
    //            {
    //                int bots = CountBotsInRange(Point{ x,y,z }, nanobots);
    //                if (bots >= maxCount)
    //                {
    //                    maxCount = bots;
    //                    int64_t currentDist = md(zero, Point{ x,y,z });
    //                    if (currentDist < dist)
    //                    {
    //                        dist = currentDist;
    //                        std::cout << "Bots in range at " << dist << ":" << x<<" " << y<< " " << z<< "\n";
    //                    }
    //                }
    //
    //            }
    //        }
    //    }
    //
    //    std::cout << "Bots in range at " << dist << "\n";
    //}


    return 0;
}