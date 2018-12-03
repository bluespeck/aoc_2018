#include <cassert>
#include <iostream>
#include <vector>

struct Claim
{
    size_t id;
    size_t x, y;
    size_t w, h;
};

using ClaimVec = std::vector<Claim>;

ClaimVec ReadInput()
{
    ClaimVec claims;

    Claim claim;
    while (std::cin.get())
    {
        std::cin >> claim.id;
        std::cin.get();
        std::cin.get();
        std::cin.get();
        std::cin >> claim.x;
        std::cin.get();
        std::cin >> claim.y;
        std::cin.get();
        std::cin >> claim.w;
        std::cin.get();
        std::cin >> claim.h;
        claims.push_back(claim);

        char dummy[16];
        if (!std::cin.getline(dummy, 16))
        {
            return claims;
        }

    }

    return claims;
}

inline bool IsInside(size_t x, size_t y, const Claim& claim)
{
    return (x >= claim.x && x < claim.x + claim.w)
        && (y >= claim.y && y < claim.y + claim.h);
}

void ProcessClaimContention(const ClaimVec& claims, size_t** fabric, size_t size)
{
    for (auto& claim : claims)
    {
        for (size_t i = 0; i < size; i++)
        {
            for (size_t j = 0; j < size; j++)
            {
                if (IsInside(i, j, claim))
                    fabric[i][j]++;
            }
        }
    }
}

size_t** AllocateFabric(size_t size)
{
    size_t **fabric = new size_t*[size];
    for (size_t i = 0; i < size; i++)
    {
        fabric[i] = new size_t[size];
        for (size_t j = 0; j < size; j++)
            fabric[i][j] = 0;
    }

    return fabric;
}

void ReleaseFabric(size_t** fabric, size_t size)
{
    for (size_t i = 0; i < size; i++)
        delete[] fabric[i];
    delete[] fabric;
}

size_t NumInchesInMultipleClaims(const ClaimVec& claims, size_t** fabric, size_t size)
{
    size_t count = 0;
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (fabric[i][j] >= 2)
                count++;
        }
    }
    return count;
}

size_t GetIdOfUncontended(const ClaimVec& claims, size_t** fabric, size_t size)
{
    for (auto& claim : claims)
    {
        size_t count = 0;
        assert(claim.x < size && claim.x + claim.w < size);
        assert(claim.y < size && claim.y + claim.h < size);

        for (size_t i = claim.x; i < claim.x + claim.w; i++)
        {
            for (size_t j = claim.y; j < claim.y + claim.h; j++)
            {
                if (fabric[i][j] > 1)
                {
                    count++;
                    break;
                }
            }
            if (count > 0)
            {
                break;
            }
        }

        if (count == 0)
        {
            return claim.id;
        }
    }

    return -1;
}

int main()
{
    ClaimVec claims = ReadInput();

    const size_t size = 1024;
    auto fabric = AllocateFabric(size);

    ProcessClaimContention(claims, fabric, size);

    std::cout << NumInchesInMultipleClaims(claims, fabric, size) << "\n";
    std::cout << GetIdOfUncontended(claims, fabric, size) << '\n';

    ReleaseFabric(fabric, size);

    return 0;
}