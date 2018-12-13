#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Cart
{
	int x, y;
	int nextIntersectionDir = 0; // 0, 1, 2; increment % 3
	int currentDir = 'v'; // v, <, >, ^
};

using Grid = std::vector<std::string>;
using CartVec = std::vector<Cart>;

std::vector<Cart> carts;

Grid ReadInput()
{
    Grid grid;
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        grid.emplace_back(std::move(line));
    }

    return grid;
}

CartVec ExtractCartsFromGrid(Grid& grid)
{
    CartVec carts;

    int dirMap[256];
    dirMap['v'] = 0;
    dirMap['<'] = 1;
    dirMap['>'] = 2;
    dirMap['^'] = 3;
    // dirMap[0] = 'v';
    // dirMap[1] = '<';
    // dirMap[2] = '>';
    // dirMap[3] = '^';

    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == 'v' || grid[i][j] == '>' || grid[i][j] == '<' || grid[i][j] == '^')
            {
                Cart cart;
                cart.x = i;
                cart.y = j;
                cart.nextIntersectionDir = 0;
                cart.currentDir = dirMap[grid[i][j]];
                carts.push_back(cart);
                if (grid[i][j] == 'v' || grid[i][j] == '^')
                    grid[i][j] = '|';
                if (grid[i][j] == '<' || grid[i][j] == '>')
                    grid[i][j] = '-';
            }
        }
    }
    return carts;
}

void SimulateTrains(const Grid& grid, CartVec& carts)
{
    int dirSwitcher[4][3] =
    {
        { 2, 0, 1 },
        { 0, 1, 3 },
        { 3, 2, 0 },
        { 1, 3, 2 }
    };

    int cornerSwitcher[4][256];
    cornerSwitcher[0]['/'] = 1;
    cornerSwitcher[1]['/'] = 0;
    cornerSwitcher[2]['/'] = 3;
    cornerSwitcher[3]['/'] = 2;
    cornerSwitcher[0]['\\'] = 2;
    cornerSwitcher[1]['\\'] = 3;
    cornerSwitcher[2]['\\'] = 0;
    cornerSwitcher[3]['\\'] = 1;

    bool firstCrashProcessed = false;

    int deltaXSwitcher[4]{ 1, 0, 0, -1 };
    int deltaYSwitcher[4]{ 0, -1, 1, 0 };

    while (carts.size())
    {
        std::sort(carts.begin(), carts.end(), [](const Cart& a, const Cart& b) { return a.x < b.x || a.x == b.x && a.y < b.y; });

        for (size_t i = 0; i < carts.size(); i++)
        {
            auto& cart = carts[i];

            int newX = cart.x + deltaXSwitcher[cart.currentDir];
            int newY = cart.y + deltaYSwitcher[cart.currentDir];

            auto it = std::find_if(carts.begin(), carts.end(), [newX, newY](const Cart& cart) { return cart.x == newX && cart.y == newY; });
            if (it != carts.end())
            {
                if (static_cast<size_t>(std::distance(carts.begin(), it)) < i)
                    --i;
                carts.erase(it);

                carts.erase(carts.begin() + i);
                --i;

                if (firstCrashProcessed == false)
                {
                    std::cout << newY << ',' << newX << '\n';
                    firstCrashProcessed = true;
                }
            }
            else
            {
                int newDir = cart.currentDir;
                switch (grid[newX][newY])
                {
                case '+':
                {
                    newDir = dirSwitcher[cart.currentDir][cart.nextIntersectionDir];
                    cart.nextIntersectionDir = (cart.nextIntersectionDir + 1) % 3;
                    break;
                }
                case '/': [[fallthrough]];
                case '\\':
                {
                    newDir = cornerSwitcher[cart.currentDir][grid[newX][newY]];
                    break;
                }
                }

                if (carts.size() == 1)
                {
                    std::cout << cart.y << ',' << cart.x << '\n';
                    return;
                }

                cart.x = newX;
                cart.y = newY;
                cart.currentDir = newDir;
            }
        }
    }
}

int main()
{
    Grid grid = ReadInput();
    CartVec carts = ExtractCartsFromGrid(grid);

    SimulateTrains(grid, carts);

	return 0;

}
