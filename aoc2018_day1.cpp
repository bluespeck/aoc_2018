#include <cstdint>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

using FreqChangeVector = std::vector<int32_t>;

int32_t GetResultingFreq(const FreqChangeVector& freqChanges)
{
    return std::accumulate(freqChanges.begin(), freqChanges.end(), 0);
}

int32_t GetFirstFreqReachedTwice(const FreqChangeVector& freqChanges)
{
    std::set<int32_t> frequencies;

    int32_t freq = 0;
    while (true)
    {
        for (int i = 0; i < freqChanges.size(); ++i)
        {
            freq += freqChanges[i];
            if (frequencies.find(freq) != frequencies.end())
            {
                return freq;
            }
            else
            {
                frequencies.insert(freq);
            }
        }
    }
}

int main()
{
    FreqChangeVector freqChanges;
    int32_t freqChange;
    while(std::cin >> freqChange)
    {
        freqChanges.push_back(freqChange);
    }

    std::cout << GetResultingFreq(freqChanges) << "\n";
    std::cout << GetFirstFreqReachedTwice(freqChanges) << "\n";

    return 0;

}