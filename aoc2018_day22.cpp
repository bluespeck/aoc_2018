#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <sstream>

using Row = std::vector<int32_t>;
using Grid = std::vector<Row>;
using ToolDists = std::array<int32_t, 3>;
using DistRow = std::vector<ToolDists>;
using DistGrid = std::vector<DistRow>;

int minTime = 50000;
int tx = 0;
int ty = 0;

int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 };

char translationTable[7] = "NCT.=|";

enum Tool
{
    torch,
    climbingGear,
    neither
};

void PrintGrid(Grid& grid)
{
    grid[ty][tx] = -1;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            std::cout << translationTable[grid[i][j] + 3];
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    grid[ty][tx] = 0;
}

void backtrack(Grid& grid, int x, int y, Tool tool, int time)
{
    if (time >= minTime)
        return;

    if (x == tx && y == ty)
    {
        if (tool != Tool::torch)
        {
            time += 7;
        }

        if (time < minTime)
        {

            PrintGrid(grid);

            minTime = time;
            std::cout << minTime << "\n";
            std::cout << "\n";
        }
        return;
    }

    int prev = grid[y][x];
    grid[y][x] = -static_cast<int>(tool) - 1;

    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx < 0 || ny < 0 || nx >= grid[x].size() || ny >= grid.size() || grid[ny][nx] == -1)
            continue;
        switch (grid[ny][nx])
        {
        case 0:
            backtrack(grid, nx, ny, Tool::climbingGear, time + (tool == Tool::climbingGear ? 1 : 8));
            backtrack(grid, nx, ny, Tool::torch, time + (tool == Tool::torch ? 1 : 8));
            break;
        case 1:
            backtrack(grid, nx, ny, Tool::climbingGear, time + (tool == Tool::climbingGear ? 1 : 8));
            backtrack(grid, nx, ny, Tool::neither, time + (tool == Tool::neither ? 1 : 8));
            break;
        case 2:
            backtrack(grid, nx, ny, Tool::torch, time + (tool == Tool::torch ? 1 : 8));
            backtrack(grid, nx, ny, Tool::neither, time + (tool == Tool::neither ? 1 : 8));
            break;
        }
    }
    grid[y][x] = prev;
}

void MinTimeCalibrationStep(const Grid& grid, int x, int y, Tool& tool)
{
    switch (grid[y][x])
    {
    case 0:
        if (tool == Tool::climbingGear || tool == Tool::torch)
        {
            minTime++;
        }
        else
        {
            minTime += 8;
            tool = Tool::climbingGear;
        }
        break;
    case 1:
        if (tool == Tool::climbingGear || tool == Tool::neither)
        {
            minTime++;
        }
        else
        {
            minTime += 8;
            tool = Tool::climbingGear;
        }
        break;
    case 2:
        if (tool == Tool::torch || tool == Tool::neither)
        {
            minTime++;
        }
        else
        {
            minTime += 8;
            tool = Tool::torch;
        }
        break;
    }
}

void CalibrateMinTime(Grid&grid)
{
    Tool tool = Tool::torch;
    minTime = 0;
    for (int i = 1; i <= tx; i++)
    {
        MinTimeCalibrationStep(grid, i, 0, tool);
    }
    for (int j = 1; j < ty; j++)
    {
        MinTimeCalibrationStep(grid, 0, j, tool);
    }
    if (tool != Tool::torch)
        minTime += 8;
}

void ComputeDistances(const Grid& grid, DistGrid& distGrid, int x, int y)
{
    const int32_t infinity = grid.size() * grid[0].size() * 8;
    static int prevMin = infinity;

    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx < 0 || ny < 0 || nx >= grid[x].size() || ny >= grid.size()
            //|| distGrid[nx][Tool::torch][nx] < infinity
            //|| distGrid[nx][Tool::climbingGear][nx] < infinity
            //|| distGrid[nx][Tool::neither][nx] < infinity
            )
            continue;

        ToolDists nDist = { infinity, infinity, infinity };

        switch (grid[ny][nx])
        {
        case 0:
            nDist[Tool::torch] = std::min(
                {
                    distGrid[y][x][Tool::torch] + 1,
                    distGrid[y][x][Tool::climbingGear] + 8,
                    distGrid[y][x][Tool::neither] + 8
                });
            nDist[Tool::climbingGear] = std::min(
                {
                    distGrid[y][x][Tool::torch] + 8,
                    distGrid[y][x][Tool::climbingGear] + 1,
                    distGrid[y][x][Tool::neither] + 8
                });
            nDist[Tool::neither] = infinity;
            break;
        case 1:
            nDist[Tool::torch] = infinity;
            nDist[Tool::climbingGear] = std::min(
                {
                    distGrid[y][x][Tool::torch] + 8,
                    distGrid[y][x][Tool::climbingGear] + 1,
                    distGrid[y][x][Tool::neither] + 8
                });
            nDist[Tool::neither] = std::min(
                {
                    distGrid[y][x][Tool::torch] + 8,
                    distGrid[y][x][Tool::climbingGear] + 8,
                    distGrid[y][x][Tool::neither] + 1
                });

            break;
        case 2:
            nDist[Tool::torch] = std::min(
                {
                    distGrid[y][x][Tool::torch] + 1,
                    distGrid[y][x][Tool::climbingGear] + 8,
                    distGrid[y][x][Tool::neither] + 8
                });
            nDist[Tool::climbingGear] = infinity;
            nDist[Tool::neither] = std::min(
                {
                    distGrid[y][x][Tool::torch] + 8,
                    distGrid[y][x][Tool::climbingGear] + 8,
                    distGrid[y][x][Tool::neither] + 1
                });
            break;
        }
        bool betterPathFound = false;
        for (int i = Tool::torch; i <= Tool::neither; i++)
        {
            if (nDist[i] < distGrid[ny][nx][i])
            {
                distGrid[ny][nx][i] = nDist[i];
                betterPathFound = true;
            }
        }
        if (betterPathFound)
        {
            if (distGrid[ty][tx][0] < prevMin)
            {
                prevMin = distGrid[ty][tx][0];
                std::cout << prevMin << " ";
            }
            ComputeDistances(grid, distGrid, nx, ny);
        }
    }
}

void ComputeDistancesNoRecursion(const Grid& grid, DistGrid& distGrid)
{
    const int32_t infinity = grid.size() * grid[0].size() * 8;
    static int prevMin = infinity;

    bool betterPathFound = false;

    do
    {
        betterPathFound = false;
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                int x = j;
                int y = i;
                for (int di = 0; di < 4; di++)
                {
                    int nx = x + dx[di];
                    int ny = y + dy[di];

                    if (nx < 0 || ny < 0 || nx >= grid[i].size() || ny >= grid.size())
                        continue;

                    ToolDists nDist = { infinity, infinity, infinity };

                    switch (grid[y][x])
                    {
                    case 0:
                        if(grid[ny][nx] != 1)
                            nDist[Tool::torch] = std::min(
                            {
                                distGrid[ny][nx][Tool::torch] + 1,
                                distGrid[ny][nx][Tool::climbingGear] + 8,
                                distGrid[ny][nx][Tool::neither] + 8
                            });
                        if (grid[ny][nx] != 2)
                        nDist[Tool::climbingGear] = std::min(
                            {
                                distGrid[ny][nx][Tool::torch] + 8,
                                distGrid[ny][nx][Tool::climbingGear] + 1,
                                distGrid[ny][nx][Tool::neither] + 8
                            });
                        nDist[Tool::neither] = infinity;
                        break;
                    case 1:
                        nDist[Tool::torch] = infinity;
                        if (grid[ny][nx] != 2)
                            nDist[Tool::climbingGear] = std::min(
                            {
                                distGrid[ny][nx][Tool::torch] + 8,
                                distGrid[ny][nx][Tool::climbingGear] + 1,
                                distGrid[ny][nx][Tool::neither] + 8
                            });
                        if (grid[ny][nx] != 0)
                            nDist[Tool::neither] = std::min(
                            {
                                distGrid[ny][nx][Tool::torch] + 8,
                                distGrid[ny][nx][Tool::climbingGear] + 8,
                                distGrid[ny][nx][Tool::neither] + 1
                            });
                        break;
                    case 2:
                        if (grid[ny][nx] != 1)
                            nDist[Tool::torch] = std::min(
                            {
                                distGrid[ny][nx][Tool::torch] + 1,
                                distGrid[ny][nx][Tool::climbingGear] + 8,
                                distGrid[ny][nx][Tool::neither] + 8
                            });
                        nDist[Tool::climbingGear] = infinity;
                        if (grid[ny][nx] != 0)
                            nDist[Tool::neither] = std::min(
                            {
                                distGrid[ny][nx][Tool::torch] + 8,
                                distGrid[ny][nx][Tool::climbingGear] + 8,
                                distGrid[ny][nx][Tool::neither] + 1
                            });
                        break;
                    default:
                        std::cout << "-----------";
                        break;
                    }
                    for (int ti = Tool::torch; ti <= Tool::neither; ti++)
                    {
                        if (nDist[ti] < distGrid[y][x][ti])
                        {
                            distGrid[y][x][ti] = nDist[ti];
                            betterPathFound = true;
                        }
                    }

                }
            }
        }

        if (betterPathFound && distGrid[ty][tx][Tool::torch] < prevMin)
        {
            prevMin = distGrid[ty][tx][Tool::torch];
            std::cout << prevMin << " ";
        }
    } while (betterPathFound);
}

DistGrid ComputeDistances(Grid& grid)
{
    const int32_t infinity = grid[0].size() * grid.size() * 8;

    DistGrid distGrid;
    distGrid.resize(grid.size());
    for(int i = 0; i < grid.size(); i++)
    {
        distGrid[i].resize(grid[i].size(), { infinity, infinity, infinity });
    }

    distGrid[0][0][Tool::torch] = 0; // row / tool type (T,C,N) / column
    ComputeDistancesNoRecursion(grid, distGrid);
    return distGrid;
}

Grid BuildGrid(int depth)
{
    Grid grid;
    int maxCoord = tx > ty ? tx : ty;
    const int extra = 10;
    grid.resize(maxCoord + extra);
    for (int i = 0; i < grid.size(); i++)
        grid[i].resize(maxCoord + extra);

    for (int i = 0; i < grid.size(); i++)
    {
        grid[i][0] = ((i * 48'271 % 20'183) % 20'183 + depth) % 20'183;
        grid[0][i] = ((i * 16'807 % 20'183) % 20'183 + depth) % 20'183;
    }
    for (int k = 1; k < grid.size(); k++)
    {
        for (int i = k; i < grid.size(); i++)
        {
            if ((i == tx && k == ty) || (i == ty && k == tx))
                grid[ty][tx] = (0 + depth) % 20'183;
            else
            {
                grid[i][k] = ((grid[i - 1][k] * grid[i][k - 1]) % 20'183 + depth) % 20'183;
                grid[k][i] = ((grid[k - 1][i] * grid[k][i - 1]) % 20'183 + depth) % 20'183;
            }
        }
    }

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            grid[i][j] %= 3;
        }
    }

    return grid;
}

void Problem1(Grid& grid)
{
    int64_t risk = 0;
    for (int i = 0; i <= ty; i++)
    {
        for (int j = 0; j <= tx; j++)
        {
            risk += grid[i][j];
        }
    }

    PrintGrid(grid);

    std::cout << risk <<"\n";
}

void PrintDistGrid(DistGrid& grid)
{
    const int32_t infinity = grid[0].size() * grid.size() * 8;
    for (int i = 0; i < grid.size(); i++)
    {
        std::cout << "|";
        for (int j = 0; j < grid[i].size(); j++)
        {
            int size = 5;
            if (i == ty && j == tx)
            {
                std::cout << "(";
                size --;
            }
            if (grid[i][j][Tool::torch] != infinity)
                std::cout << std::setw(size) << grid[i][j][Tool::torch];
            else
                    std::cout << std::setw(size) << "--";

            if (grid[i][j][Tool::climbingGear] != infinity)
                std::cout << std::setw(size) << grid[i][j][Tool::climbingGear];
            else
                std::cout << std::setw(size) << "--";

            if (grid[i][j][Tool::neither] != infinity)
                std::cout << std::setw(size) << grid[i][j][Tool::neither];
            else
                std::cout << std::setw(size) << "--";
            if (i == ty && j == tx)
                std::cout << " )";
            std::cout << "|";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

}

#pragma optimize("", off)
void PrintSolutionGrids(DistGrid& grid, Grid mainGrid)
{
    const int32_t infinity = grid[0].size() * grid.size() * 8;

    std::vector<std::pair<std::pair<int, int>, int>> path;
    int x = tx;
    int y = ty;
    int t = 0;
    path.emplace_back(std::make_pair(x, y), t);



    while (x != 0 || y != 0)
    {
        bool found = false;

        for (int di = 0; !found && di < 4; di++)
        {
            int nx = x + dx[di];
            int ny = y + dy[di];
            if (nx < 0 || ny < 0 || nx >= grid[y].size() || ny >= grid.size())
                continue;
            for (int tj = Tool::torch; !found && tj <= Tool::neither; tj++)
                if (grid[ny][nx][tj] != infinity)
                {
                    if ((tj == t && grid[y][x][t] == grid[ny][nx][tj] + 1)
                        || (tj != t && grid[y][x][t] == grid[ny][nx][tj] + 8)
                        )
                    {
                        x = nx;
                        y = ny;
                        t = tj;
                        found = true;
                    }
                }
        }
        assert(found == true);
        path.emplace_back(std::make_pair(x, y), t);
    }
    std::reverse(path.begin(), path.end());
    int tool = 0;
    int time = -1;
    int prevX = 0;
    int prevY = 0;
    for (auto& elem : path)
    {
        int cellType = mainGrid[elem.first.second][elem.first.first];
        if (cellType == 0 && elem.second == 2 || cellType == 1 && elem.second == 0 || cellType == 2 && elem.second == 1)
        {
            std::cout << "--------------\n";
        }
        if (tool != elem.second)
        {
            int prevCellType = mainGrid[prevY][prevX];
            if ((prevCellType == 0 && elem.second == 2)
                || (prevCellType == 1 && elem.second == 0)
                || prevCellType == 2 && elem.second == 1)
            {
                std::cout << "--------------\n";
            }
            time += 8;
            tool = elem.second;
        }
        else
            time++;
        prevX = elem.first.first;
        prevY = elem.first.second;
    }
    std::cout << "Time : " << time << "\n";

    for (auto& elem : path)
    {
        mainGrid[elem.first.second][elem.first.first] = -elem.second - 1;
    }
    PrintGrid(mainGrid);

    for (int i = 0; i < grid.size(); i++)
    {
        std::cout << "|";
        for (int j = 0; j < grid[i].size(); j++)
        {
            int size = 5;
            if (i == ty && j == tx)
            {
                std::cout << "(";
                size--;
            }
            int t = infinity;
            if (auto it = std::find_if(path.begin(), path.end(),
                    [j, i](const auto& elem)
                    {
                        return elem.first.first == j && elem.first.second == i;
                    }); it != path.end())
            {
                t = it->second;
                size--;
            }
            if (t == Tool::torch)
            {
                std::cout << "  *";
            }
            if (grid[i][j][Tool::torch] != infinity)
                std::cout << std::setw(size) << grid[i][j][Tool::torch];
            else
                std::cout << std::setw(size) << "--";

            if (t == Tool::climbingGear)
            {
                std::cout << "  *";
            }
            if (grid[i][j][Tool::climbingGear] != infinity)
                std::cout << std::setw(size) << grid[i][j][Tool::climbingGear];
            else
                std::cout << std::setw(size) << "--";

            if (t == Tool::neither)
            {
                std::cout << "  *";
            }
            if (grid[i][j][Tool::neither] != infinity)
                std::cout << std::setw(size) << grid[i][j][Tool::neither];
            else
                std::cout << std::setw(size) << "--";
            if (i == ty && j == tx)
                std::cout << " )";
            std::cout << "|";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

}

#pragma optimize("", on)

int main()
{
    int depth = 10689;
    tx = 11;
    ty = 722;

    //int depth = 510;
    //tx = 10;
    //ty = 10;

    Grid grid = BuildGrid(depth);

    //Problem1(grid);

    DistGrid distGrid = ComputeDistances(grid);
    std::cout << distGrid[ty][tx][Tool::torch] << "\n";
    PrintGrid(grid);
//    PrintDistGrid(distGrid);
    PrintSolutionGrids(distGrid, grid);
    return 0;
}
