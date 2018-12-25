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

struct Coord
{
    int x, y, z, w;
};
//using Grid = std::array<std::array<std::array<std::array<int, 20>, 20>, 20>, 20>;

using CoordVec = std::vector<Coord>;

void ReadInput(CoordVec& coords)
{
    //Grid grid;

    //for(int i = 0; i < grid.size(); i++)
    //    for (int j = 0; j < grid[i].size(); j++)
    //        for (int k = 0; k < grid[i][j].size(); k++)
    //            for (int l = 0; l < grid[i][j][k].size(); l++)
    //            {
    //                grid[i][j][k][l] = 0;
    //            }
    //

    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (std::cin)
        {
            std::istringstream is(line);
            char c;
            int x, y, z, w;
            is >> x >> c >> y >>c >> z >>c >> w;
            coords.push_back(Coord{ x,y,z,w });
            x += 10;
            y += 10;
            z += 10;
            w += 10;
            //grid[x][y][z][w] = 1;
        }
    }

    //return grid;
}

int md(int x1, int y1, int z1, int w1, int x2, int y2, int z2, int w2)
{
    return std::abs(x1 - x2) + std::abs(y1 - y2) + std::abs(z1 - z2) + std::abs(w1 - w2);
}

int md(Coord c1, Coord c2)
{
    return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y) + std::abs(c1.z - c2.z) + std::abs(c1.w - c2.w);
}

int Connect(CoordVec& coords)
{
    std::vector<std::vector<int>> distances(coords.size());

    for (int i = 0; i < distances.size(); i++)
    {
        distances[i].resize(distances.size());
    }
    for (int i = 0; i < distances.size(); i++)
    {
        for (int j = i; j < distances[i].size(); j++)
        {
            distances[i][j] = distances[j][i] = md(coords[i], coords[j]);
        }
    }

    {
        std::vector<std::vector<int>> constellations;
        int currentComponent = -1;
        int currentNode = -1;
        std::set<int> visited;

        while (visited.size() < coords.size())
        {
            currentComponent++;
            constellations.push_back(std::vector<int>());

            for (int i = 0; i < coords.size(); i++)
            {
                if (visited.count(i) == 0)
                {
                    currentNode = i;
                    visited.insert(i);
                    constellations.back().push_back(currentNode);
                    break;
                }
            }

            while (true)
            {
                bool constellationGrew = false;
                for (int i = 0; i < coords.size(); i++)
                {
                    if (visited.count(i) == 0)
                    {
                        bool canConnect = false;
                        for (int j = 0; j < constellations.back().size(); j++)
                        {
                            if (distances[i][constellations.back()[j]] <= 3)
                            {
                                canConnect = true;
                                break;
                            }
                        }
                        if (canConnect)
                        {
                            currentNode = i;
                            visited.insert(i);
                            constellations.back().push_back(i);
                            constellationGrew = true;
                        }
                    }
                }
                if (constellationGrew == false)
                    break;
            }
        }
        return constellations.size();
    }
}

int main()
{
    //Grid grid;
    CoordVec coords;
    ReadInput(coords);
    std::cout << Connect(coords);

    return 0;
}