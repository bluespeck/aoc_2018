#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

using Registers = std::array<int64_t, 6>;

struct Instruction
{
    int opcode;
    int a, b, c;
};

using Program = std::vector<Instruction>;

void addr(int a, int b, int c, Registers& reg)
{
    reg[c] = reg[a] + reg[b];
}

void addi(int a, int b, int c, Registers&reg)
{
    reg[c] = reg[a] + b;
}

void mulr(int a, int b, int c, Registers&reg)
{
    reg[c] = reg[a] * reg[b];
}

void muli(int a, int b, int c, Registers&reg)
{
    reg[c] = reg[a] * b;
}

void banr(int a, int b, int c, Registers&reg)
{
    reg[c] = reg[a] & reg[b];
}

void bani(int a, int b, int c, Registers&reg)
{
    reg[c] = reg[a] & b;
}

void borr(int a, int b, int c, Registers&reg)
{
    reg[c] = reg[a] | reg[b];
}

void bori(int a, int b, int c, Registers&reg)
{
    reg[c] = reg[a] | b;
}

void setr(int a, int b, int c, Registers&reg)
{
    reg[c] = reg[a];
}

void seti(int a, int b, int c, Registers&reg)
{
    reg[c] = a;
}

void gtir(int a, int b, int c, Registers&reg)
{
    reg[c] = (a > reg[b]) ? 1 : 0;
}

void gtri(int a, int b, int c, Registers&reg)
{
    reg[c] = (reg[a] > b) ? 1 : 0;
}

void gtrr(int a, int b, int c, Registers&reg)
{
    reg[c] = (reg[a] > reg[b]) ? 1 : 0;
}

void eqir(int a, int b, int c, Registers&reg)
{
    reg[c] = (a == reg[b]) ? 1 : 0;
}

void eqri(int a, int b, int c, Registers&reg)
{
    reg[c] = (reg[a] == b) ? 1 : 0;
}

void eqrr(int a, int b, int c, Registers&reg)
{
    reg[c] = (reg[a] == reg[b]) ? 1 : 0;
}

using PossibleOpcodeSets = std::array<std::set<int>, 16>;
using Op = void(*)(int, int, int, Registers&);
using OpcodeTranslationTable = std::array<int, 16>;

std::array<Op, 16> operations{
    addr, addi,
    mulr, muli,
    banr, bani,
    borr, bori,
    setr, seti,
    gtir, gtri, gtrr,
    eqir, eqri, eqrr
};

int64_t RunProgram(const Program& program, Registers& regs, int ipr, int maxNumSteps = 0)
{
    int64_t numSteps = 0;
    while (regs[ipr] < program.size())
    {
        auto& instruction = program[regs[ipr]];
        {
            std::cout << "ip=" << regs[ipr];
            std::cout << " [";
            for (int i = 0; i < 5; i++)
                std::cout << regs[i] << ", ";
            std::cout << regs[5] << "] " << instruction.opcode << " " << instruction.a << " " << instruction.b << " " << instruction.c << " ";
        }
        operations[instruction.opcode](instruction.a, instruction.b, instruction.c, regs);

        {
            std::cout << " [";
            for (int i = 0; i < 5; i++)
                std::cout << regs[i] << ", ";
            std::cout << regs[5] << "]\n";
        }

        regs[ipr] ++;
		numSteps++;

    }
	return numSteps;
}

void ReadInput(Program& program, int& ip)
{
    std::cin.ignore(4);
    std::cin >> ip;
    std::map<std::string, int> nameToOpcode;
    nameToOpcode["addr"] = 0;
    nameToOpcode["addi"] = 1;
    nameToOpcode["mulr"] = 2;
    nameToOpcode["muli"] = 3;
    nameToOpcode["banr"] = 4;
    nameToOpcode["bani"] = 5;
    nameToOpcode["borr"] = 6;
    nameToOpcode["bori"] = 7;
    nameToOpcode["setr"] = 8;
    nameToOpcode["seti"] = 9;
    nameToOpcode["gtir"] = 10;
    nameToOpcode["gtri"] = 11;
    nameToOpcode["gtrr"] = 12;
    nameToOpcode["eqir"] = 13;
    nameToOpcode["eqri"] = 14;
    nameToOpcode["eqrr"] = 15;

    while (std::cin)
    {
        std::string instructionName;
        Instruction instruction;
        std::cin >> instructionName >> instruction.a >> instruction.b >> instruction.c;
        if (std::cin)
        {
            instruction.opcode = nameToOpcode[instructionName];
            program.push_back(instruction);
        }

    }
}

using Regs25Vec = std::vector<std::pair<int64_t, int64_t>>;

Regs25Vec SimulateReg2AndReg5()
{
	int64_t reg2 = 15608036;
	int64_t reg5 = 65536;

	Regs25Vec regPairs;

	while (true)
	{
		if (reg5 < 256)
		{
			regPairs.push_back(std::make_pair(reg2, reg5));
		}

		if (reg5 < 256)
		{
			reg5 = reg2 | 65536;
			reg2 = (5234604 + (reg5 & 255)) & 0x00ff'ffff;
			reg2 = (reg2 * 65899) & 0x00ff'ffff;

		}
		else
		{
			reg5 /= 256;

			// bani 5 255 3
			// addr 2 3 2
			// bani 2 16777215 2
			// muli 2 65899 2
			// bani 2 16777215 2

			reg2 = (reg2 + (reg5 & 255)) & 0x00ff'ffff;
			reg2 = (reg2 * 65899) & 0x00ff'ffff;
		}
		auto it = std::find_if(regPairs.begin(), regPairs.end(), [reg2, reg5](auto& elem) { return elem.first == reg2 && elem.second == reg5; });
		if (it != regPairs.end())
		{
			break;
		}

	}

	return regPairs;
}

int main()
{
    //Program program;
    //int ipr = 0;
    //ReadInput(program, ipr);
	//
	//{
	//    Registers regs = { 0, 0, 0, 0, 0, 0 };
	//    int64_t steps = RunProgram(program, regs, ipr);
	//    std::cout << "\n" << steps << "\n";
	//}

	auto regPairs = SimulateReg2AndReg5();

	std::cout << regPairs[0].first << "\n";

	for (int i = regPairs.size() - 1; i >= 0; --i)
	{
		auto current = regPairs[i].first;
		auto it = std::find_if(regPairs.begin(), regPairs.begin() + i, [current](const std::pair<int64_t, int64_t>& elem) { return elem.first == current; });
		if (it == regPairs.begin() + i)
		{
			std::cout << regPairs[i].first << "\n";
			break;
		}
	}

    return 0;
}