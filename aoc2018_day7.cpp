#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using Edge = std::pair<char, char>;
using EdgeVec = std::vector < Edge>;
using StepVec = std::vector<char>;
using StepSet = std::set<char>;
using AdjacencyMatrix = std::array<std::array<size_t, 26>, 26>;

void ReadInput(EdgeVec& edges, AdjacencyMatrix& adjacency, StepSet& vertices)
{
    do
    {
        Edge edge;
        std::cin.ignore(5);
        std::cin >> edge.first;
        std::cin.ignore(30);
        std::cin >> edge.second;
        std::cin.ignore(12);
        if (edge.first >= 'A' && edge.second <= 'Z')
        {
            edges.push_back(edge);

            vertices.insert(edge.first);
            vertices.insert(edge.second);

            adjacency[edge.first - 'A'][edge.second - 'A'] = 1;
        }
    } while (std::cin);
}

size_t inDeg(const AdjacencyMatrix& adj, size_t vertex)
{
    size_t sum = 0;
    for (size_t i = 0; i < 26; i++)
        sum += adj[i][vertex];
    return sum;
}

StepVec OrderSequence(AdjacencyMatrix adjacency, StepSet vertices)
{
    StepVec sequence;

    while (vertices.size() > 0)
    {
        StepVec toVisit;
        for (auto v : vertices)
        {
            if (inDeg(adjacency, v-'A') == 0)
            {
                toVisit.push_back(v);
            }
        }

        std::sort(toVisit.begin(), toVisit.end());

        char c = toVisit.front();
        sequence.push_back(c);
        adjacency[c - 'A'].fill(0);
        vertices.erase(c);

        toVisit.clear();
    }

    return sequence;
}

size_t TimeToCompleteStepsByFiveWorkers(StepVec& sequence, AdjacencyMatrix adjacency)
{
    size_t time = 0;

    const size_t numWorkers = 5;

    int worker[numWorkers] = { 0 };
    char workerTask[numWorkers] = { 0 };

    while (true)
    {
        bool activeWorkers = false;
        for (size_t w = 0; w < numWorkers; w++)
        {
            activeWorkers = (activeWorkers || (worker[w] > 0));
        }
        if (activeWorkers == false && sequence.size() == 0)
            break;

        for (size_t w = 0; w < numWorkers; w++)
        {
            if (worker[w] == 0)
            {
                if (workerTask[w] > 0)
                {
                    adjacency[workerTask[w] - 'A'].fill(0);
                    workerTask[w] = 0;
                }

                if (sequence.size() > 0)
                {
                    for (auto sequenceIt = sequence.begin(); sequenceIt != sequence.end(); sequenceIt++)
                    {
                        char c = *sequenceIt;
                        if (inDeg(adjacency, c - 'A') == 0)
                        {
                            workerTask[w] = c;
                            worker[w] = 60 + c - 'A' + 1;
                            sequence.erase(sequenceIt);
                            break;
                        }
                    }
                }
            }
        }
        for (size_t w = 0; w < numWorkers; w++)
        {
            if (worker[w] > 0)
            {
                worker[w] --;
            }
        }
        //std::cout << time;
        //for (size_t w = 0; w < numWorkers; w++)
        //    std::cout << workerTask[w];
        //std::cout << '\n';
        time++;
    }

    return time;
}

int main()
{
    EdgeVec edges;
    StepSet vertices;
    AdjacencyMatrix adjacency;

    for (size_t i = 0; i < adjacency.size(); i++)
    {
        adjacency[i].fill(0);
    }

    ReadInput(edges, adjacency, vertices);

    auto sequence = OrderSequence(adjacency, vertices);

    for (char c : sequence)
    {
        std::cout << c;
    }
    std::cout << '\n';

    size_t time = TimeToCompleteStepsByFiveWorkers(sequence, adjacency);

    std::cout << time << '\n';

    return 0;
}