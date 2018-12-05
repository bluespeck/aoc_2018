#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::string ReadInput()
{
    std::string polymer;
    std::cin >> polymer;
    return polymer;
}

int ReactPolymer(std::string polymer)
{
    bool reacted = false;

    do
    {
        reacted = false;
        for (size_t i = 0; i < polymer.size() - 1; )
        {
            if (polymer[i] == polymer[i + 1] + 0x20 || polymer[i] == polymer[i + 1] - 0x20)
            {
                reacted = true;
                for (size_t j = i; j < polymer.size() - 2; j++)
                    polymer[j] = polymer[j + 2];
                polymer.resize(polymer.size() - 2);
            }
            else
            {
                i++;
            }
        }
    } while (reacted);

    return polymer.size();
}

int ShortestLengthAfterOneElementRemoval(std::string polymer)
{
    size_t minLength = polymer.size();
    for (char type = 'A'; type <= 'Z'; type++)
    {
        std::string temp = polymer;
        temp.erase(std::remove_if(temp.begin(), temp.end(), [type](char a) { return a == type || a == type + 0x20; }), temp.end());
        size_t length = ReactPolymer(temp);
        if (length < minLength)
            minLength = length;
    }
    return minLength;
}

int main()
{
    std::string polymer = ReadInput();
    std::cout << ReactPolymer(polymer) << '\n';
    std::cout << ShortestLengthAfterOneElementRemoval(polymer) << '\n';

    return 0;
}