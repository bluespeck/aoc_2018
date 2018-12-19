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

void RunProgram(const Program& program, Registers& regs, int ipr, int maxNumSteps = 0)
{
    int step = 0;
    while(regs[ipr] < program.size())
    {
        if (maxNumSteps > 0 && step >= maxNumSteps)
        {
            break;
        }
        auto& instruction = program[regs[ipr]];

        //{
        //    std::cout << "ip=" << regs[ipr];
        //    std::cout << " [";
        //    for (int i = 0; i < 5; i++)
        //        std::cout << regs[i] << ", ";
        //    std::cout << regs[5] << "] " << instruction.opcode << " " << instruction.a << " " << instruction.b << " " << instruction.c << " ";
        //}
        operations[instruction.opcode](instruction.a, instruction.b, instruction.c, regs);

        //{
        //    std::cout << " [";
        //    for (int i = 0; i < 5; i++)
        //        std::cout << regs[i] << ", ";
        //    std::cout << regs[5] << "]\n";
        //}

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

int64_t SumOfDivisors(int64_t number)
{
    int64_t sum = 1;
    for (int64_t i = 2; i <= number / 2 + 1; i++)
        if (number % i == 0)
            sum += i;
    sum += number;
    return sum;
}

int main()
{
    Program program;
    int ipr = 0;
    ReadInput(program, ipr);

    {
        Registers regs = { 0, 0, 0, 0, 0, 0 };
        RunProgram(program, regs, ipr);
        std::cout << "\n" << regs[0] << "\n";
    }


    {
        Registers regs = { 1, 0, 0, 0, 0, 0 };
        RunProgram(program, regs, ipr, 1000);
        if (regs[ipr] < program.size())
        {
            std::cout << SumOfDivisors(regs[2]) << "\n";
        }
    }

    return 0;
}