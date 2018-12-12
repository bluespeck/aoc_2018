#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using RuleVec = std::vector<std::pair<std::string, char>>;

void ReadInput(std::string& pots, RuleVec& rules)
{
	std::cin.ignore(15);

	std::cin >> pots;
	std::cin.ignore(2);

	while (std::cin)
	{
		char pattern[6] = { 0 };
		char c;
		for (int i = 0; i < 5; i++)
			std::cin >> pattern[i];

		std::cin.ignore(4);
		std::cin >> c;

		if (std::cin)
		{
			rules.emplace_back(pattern, c);
		}

		std::cin.ignore(1);
	}

}

std::string ProduceNextGen(const std::string& pots, const RuleVec& rules)
{
	std::string nextGen(pots.size(), '.');

	for (uint64_t i = 0; i < rules.size(); i++)
	{
		if (rules[i].second == '#')
		{
			uint64_t searchOffset = 0;
			while (true)
			{
				uint64_t pos = pots.find(rules[i].first, searchOffset);
				if (pos != std::string::npos)
				{
					nextGen[pos + 2] = '#';
					searchOffset = pos + 1;
				}
				else
				{
					break;
				}
			}
		}
	}

	return nextGen;
}

void PadPotsForEveryGeneration(std::string& pots, uint64_t numGenerations)
{
	std::string prefix;
	prefix.resize(numGenerations + 2, '.');
	pots = prefix + pots;
	pots.resize(pots.size() + 2 * numGenerations + 2 * 2, '.');
}

void PrintPotSumAfter20Generations(std::string pots, const RuleVec& rules)
{
	uint64_t numGen = 20;
	uint64_t indexZero = numGen + 2;

	PadPotsForEveryGeneration(pots, numGen);

	std::cout << 0 << ": " << pots << '\n';

	for (uint64_t k = 1; k <= numGen; k++)
	{
		pots = ProduceNextGen(pots, rules);
		std::cout << k + 1 << ": " << pots << '\n';
	}

	int64_t sum = 0;
	for (uint64_t i = 0; i < pots.size(); i++)
	{
		if (pots[i] == '#')
		{
			sum += i - indexZero;
		}
	}

	std::cout << '\n' << sum << '\n';
}

void PrintPotSumAfter50BillionGenerations(std::string pots, const RuleVec& rules)
{
	uint64_t numGen = 200;
	uint64_t indexZero = numGen + 2;
	uint64_t genOfConfigStabilization = 0;

	PadPotsForEveryGeneration(pots, numGen);

	std::cout << 0 << ": " << pots << '\n';

	for (uint64_t k = 1; k <= numGen; k++)
	{
		auto nextGen = ProduceNextGen(pots, rules);
		std::cout << k + 1 << ": " << nextGen << '\n';

		bool configStabilized = pots.find(nextGen.substr(1)) != std::string::npos;
		if (!configStabilized)
		{
			pots = nextGen;
		}
		else
		{
			genOfConfigStabilization = k;
			break;
		}

	}

	int64_t sum = 0;
	for (uint64_t i = 0; i < pots.size(); i++)
	{
		if (pots[i] == '#')
		{
			sum += i - indexZero + 50'000'000'000 - (genOfConfigStabilization - 1);
		}
	}

	std::cout << '\n' << sum << '\n';
}

int main()
{
	std::string pots;
	RuleVec rules;
	
	ReadInput(pots, rules);
	
	PrintPotSumAfter20Generations(pots, rules);

	PrintPotSumAfter50BillionGenerations(pots, rules);

    return 0;
}