#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <sstream>

using Registers = std::array<int, 4>;

struct Sample
{
    Registers before;
    Registers after;
    int opcode;
    int a, b, c;
};

using SampleVec = std::vector<Sample>;

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

std::array<Op, 16> operations {
    addr, addi,
    mulr, muli,
    banr, bani,
    borr, bori,
    setr, seti,
    gtir, gtri, gtrr,
    eqir, eqri, eqrr
};

int CountSamplesMatchingThreeOpsOrMore(const SampleVec& samples)
{
    int sampleCounter = 0;
    for (const auto& sample : samples)
    {
        int opCounter = 0;
        for (auto op : operations)
        {
            Registers before = sample.before;
            op(sample.a, sample.b, sample.c, before);
            if (before[sample.c] == sample.after[sample.c])
                opCounter++;
        }
        if (opCounter >= 3)
        {
            sampleCounter++;
        }
    }
    return sampleCounter;
}

void FindPossibleOpcodes(const SampleVec& samples, PossibleOpcodeSets& possibleOpcodeIndices)
{

    for (const auto& sample : samples)
    {
        std::set<int> possibleOps;
        int opCounter = 0;
        int i = 0;
        for (auto op : operations)
        {

            Registers before = sample.before;
            op(sample.a, sample.b, sample.c, before);
            if (before[sample.c] == sample.after[sample.c])
            {
                possibleOps.insert(i);
            }
            i++;
        }

        if (possibleOpcodeIndices[sample.opcode].size() > 0)
        {
            std::set<int> dest;
            std::set_intersection(possibleOpcodeIndices[sample.opcode].begin(), possibleOpcodeIndices[sample.opcode].end(), possibleOps.begin(), possibleOps.end(), std::inserter(dest, dest.begin()));
            possibleOpcodeIndices[sample.opcode] = dest;
        }
        else
        {
            possibleOpcodeIndices[sample.opcode] = possibleOps;
        }
    }
}

void BuildOpcodeTranslationTable(const SampleVec& samples, OpcodeTranslationTable& opcodeTranslationTable)
{
    PossibleOpcodeSets possibleOpcodeSets;
    FindPossibleOpcodes(samples, possibleOpcodeSets);

    bool allOpcodesFound = false;
    while (!allOpcodesFound)
    {
        allOpcodesFound = true;
        for (auto& opSet : possibleOpcodeSets)
        {
            if (opSet.size() == 1)
            {
                int opIndex = *opSet.begin();
                for (auto& otherSet : possibleOpcodeSets)
                {
                    if (otherSet.size() > 1 && otherSet.count(opIndex) > 0)
                    {
                        otherSet.erase(opIndex);
                    }
                }
            }
            else
            {
                allOpcodesFound = false;
            }
        }
    }
    for (size_t i = 0; i < possibleOpcodeSets.size(); i++)
    {
        opcodeTranslationTable[i] = *possibleOpcodeSets[i].begin();
    }
}

void RunProgram(const Program& program, Registers& regs, const OpcodeTranslationTable& opcodeTranslationTable)
{
    for (auto& instruction : program)
    {
        auto opIndex = opcodeTranslationTable[instruction.opcode];
        operations[opIndex](instruction.a, instruction.b, instruction.c, regs);
    }
}

void ReadInput(SampleVec& samples, Program& program)
{
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size() == 0)
        {
            std::getline(std::cin, line);
            if (line.size() == 0)
            {
                while (std::cin)
                {
                    Instruction instruction;
                    std::cin >> instruction.opcode >> instruction.a >> instruction.b >> instruction.c;
                    if (std::cin)
                        program.push_back(instruction);
                }
                break;
            }
        }
        else {
            std::istringstream in(line);
            Sample sample;
            char c;
            in.ignore(std::string("Before: [").size());
            in >> sample.before[0] >> c >> sample.before[1] >> c >> sample.before[2] >> c >> sample.before[3] >> c;
            std::cin >> sample.opcode >> sample.a >> sample.b >> sample.c;
            std::cin.ignore(1);
            std::cin.ignore(std::string("After:  [").size());
            std::cin >> sample.after[0] >> c >> sample.after[1] >> c >> sample.after[2] >> c >> sample.after[3] >> c;
            std::cin.ignore(2);
            samples.push_back(sample);
        }
    }
}

int main()
{
    SampleVec samples;
    Program program;

    ReadInput(samples, program);

    std::cout << CountSamplesMatchingThreeOpsOrMore(samples) << "\n";

    OpcodeTranslationTable opcodeTranslationTable;
    BuildOpcodeTranslationTable(samples, opcodeTranslationTable);

    Registers regs = { 0, 0, 0, 0 };
    RunProgram(program, regs, opcodeTranslationTable);

    std::cout << regs[0] << "\n";

    return 0;
}