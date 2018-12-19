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

void RunProgram(const Program& program, Registers& regs, int ipr, int numSteps)
{
    bool debug = false;
    int step = 0;
    int64_t prevValue = 0;
    while(regs[ipr] < program.size())
    {
        if (regs[0] != prevValue)
        {
            std::cout << regs[0] << " ";
            prevValue = regs[0];
        }
        auto& instruction = program[regs[ipr]];
        if (debug)
        {
            std::cout << "ip=" << regs[ipr];
            std::cout << " [";
            for (int i = 0; i < 5; i++)
                std::cout << regs[i] << ", ";
            std::cout << regs[5] << "] " << instruction.opcode << " " << instruction.a << " " << instruction.b << " " << instruction.c << " ";
        }
        operations[instruction.opcode](instruction.a, instruction.b, instruction.c, regs);

        if (debug)
        {
            std::cout << " [";
            for (int i = 0; i < 5; i++)
                std::cout << regs[i] << ", ";
            std::cout << regs[5] << "]\n";
        }
        regs[ipr] ++;
        step++;

    }
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

int main()
{
    Program program;
    int ipr = 0;
    ReadInput(program, ipr);

    {
        Registers regs = { 0, 0, 0, 0, 0, 0 };
        RunProgram(program, regs, ipr, 22);
        std::cout << "\n" << regs[0] << "\n";
    }


    {
        Registers regs = { 1, 0, 0, 0, 0, 0 };
        RunProgram(program, regs, ipr, 22);
        // problem 2 (prog E(div reg-2))
    }

    return 0;
}