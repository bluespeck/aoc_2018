#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>


struct TimeInterval
{
    int start, end;
};

struct Guard
{
    int guardId = -1;
    std::vector<TimeInterval> timeIntervals;
};

using GuardSleepIntervalsMap = std::map<int, Guard>;
using GuardTotalSleepMap = std::map<int, int>;

struct Timestamp
{
    int year; // 1518
    int month; // 1-12
    int day; // 1-30
    int hour; // 0-24
    int minute; // 0

};

int DiffTimeStamp(const Timestamp& time1, const Timestamp& time2)
{
    return (time1.day - time2.hour) * 24 * 60 +  (time1.hour - time2.hour) * 60 + (time1.minute - time2.minute);
}

struct Entry
{
    Timestamp timestamp;
    int guardId; // -1 unknown
    enum class Action
    {
        beginShift,
        fallAsleep,
        wakeUp
    } action;
};

using EntryVec = std::vector<Entry>;
using GuardVec = std::vector<Guard>;

EntryVec ReadInput()
{
    EntryVec entries;

    while (std::cin.get())
    {
        Entry entry;
        std::cin >> entry.timestamp.year;
        std::cin.get();
        std::cin >> entry.timestamp.month;
        std::cin.get();
        std::cin >> entry.timestamp.day;
        std::cin.get();
        std::cin >> entry.timestamp.hour;
        std::cin.get();
        std::cin >> entry.timestamp.minute;
        std::cin.get();
        std::cin.get();

        char actionStr[32];
        if (!std::cin.getline(actionStr, 32))
            break;
        std::string str{ actionStr };

        if(auto pos = str.find('#'); pos != std::string::npos )
        {
            sscanf_s(&str[pos + 1 ], "%d", &entry.guardId);
            entry.action = Entry::Action::beginShift;
        }
        else if (str[0] == 'f')
        {
            entry.guardId = -1;
            entry.action = Entry::Action::fallAsleep;
        }
        else
        {
            entry.guardId = -1;
            entry.action = Entry::Action::wakeUp;
        }
        entries.push_back(entry);
    }


    return entries;
}

void ProcessEntries(EntryVec& entries)
{
    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b)
    {
        if (a.timestamp.month < b.timestamp.month)
            return true;
        if (a.timestamp.month > b.timestamp.month)
            return false;

        if (a.timestamp.day < b.timestamp.day)
            return true;
        if (a.timestamp.day > b.timestamp.day)
            return false;

        if (a.timestamp.hour < b.timestamp.hour)
            return true;
        if (a.timestamp.hour > b.timestamp.hour)
            return false;

        if (a.timestamp.minute < b.timestamp.minute)
            return true;
        if (a.timestamp.minute > b.timestamp.minute)
            return false;

        return true;
    });

    // adjust guard ids
    int currentGuardId = -1;
    for (Entry& entry : entries)
    {
        if (entry.guardId != -1)
            currentGuardId = entry.guardId;
        else
        {
            entry.guardId = currentGuardId;
        }
    }
}

std::pair<int, int> MaxSleptMinuteAndSleepSessionCount(int guardId, const GuardSleepIntervalsMap& guardSleepIntervals)
{
    int minutesSleptEachSession[60] = { 0 };

    for (auto& timeInterval : guardSleepIntervals.at(guardId).timeIntervals)
    {
        for (int i = timeInterval.start; i < timeInterval.end; i++)
            minutesSleptEachSession[i]++;
    }

    int bestMinute = -1;
    int bestMinuteCount = 0;
    for (int i = 0; i < 60; ++i)
        if (bestMinuteCount < minutesSleptEachSession[i])
        {
            bestMinuteCount = minutesSleptEachSession[i];
            bestMinute = i;
        }

    return std::pair(bestMinute, bestMinuteCount);
}

void ComputeSleepTimes(EntryVec& entries, GuardSleepIntervalsMap& guardSleepIntervals, GuardTotalSleepMap &guardTotalSleepTimes)
{
    for (int i = 0; i < entries.size(); i++)
    {
        const Entry& entry = entries[i];

        if (entry.action == Entry::Action::fallAsleep)
        {
            int sleeptime = DiffTimeStamp(entries[i + 1].timestamp, entry.timestamp);
            if (guardTotalSleepTimes.find(entry.guardId) != guardTotalSleepTimes.end())
            {
                guardTotalSleepTimes[entry.guardId] += sleeptime;
            }
            else
            {
                guardTotalSleepTimes[entry.guardId] = sleeptime;
            }
            guardSleepIntervals[entry.guardId].timeIntervals.push_back({ entry.timestamp.minute, entries[i + 1].timestamp.minute });
        }
    }
}

int FindSleepiestGuardId(const GuardTotalSleepMap& guardTotalSleepDurations)
{
    int maxSleepTime = 0;
    int maxGuardId = -1;

    for (auto it = guardTotalSleepDurations.begin(); it != guardTotalSleepDurations.end(); it++)
    {
        if (it->second > maxSleepTime)
        {
            maxSleepTime = it->second;
            maxGuardId = it->first;
        }
    }

    return maxGuardId;
}

void PrintSleepiestGuardAndMinuteCombo(const EntryVec& entries, const GuardSleepIntervalsMap& guardSleepIntervals, const GuardTotalSleepMap& guardTotalSleepDurations)
{
    int maxGuardId = FindSleepiestGuardId(guardTotalSleepDurations);
    int bestMinute = MaxSleptMinuteAndSleepSessionCount(maxGuardId, guardSleepIntervals).first;
    std::cout << maxGuardId * bestMinute << "\n";
}

void PrintMaxSleptMinuteAndGuardCombo(const EntryVec& entries, const GuardSleepIntervalsMap& guardSleepIntervals)
{
    int maxGuardId = -1;
    int maxMinuteCount = 0;
    int maxMinute = -1;
    for (auto it = guardSleepIntervals.begin(); it != guardSleepIntervals.end(); ++it)
    {
        auto[bestMinute, bestMinuteCount] = MaxSleptMinuteAndSleepSessionCount(it->first, guardSleepIntervals);
        if (bestMinuteCount > maxMinuteCount)
        {
            maxMinuteCount = bestMinuteCount;
            maxMinute = bestMinute;
            maxGuardId = it->first;
        }
    }

    std::cout << maxGuardId * maxMinute << "\n";
}

int main()
{
    EntryVec entries = ReadInput();

    ProcessEntries(entries);
    
    GuardSleepIntervalsMap guardSleepIntervals;
    GuardTotalSleepMap guardTotalSleepDurations;

    ComputeSleepTimes(entries, guardSleepIntervals, guardTotalSleepDurations);
    
    PrintSleepiestGuardAndMinuteCombo(entries, guardSleepIntervals, guardTotalSleepDurations);
    PrintMaxSleptMinuteAndGuardCombo(entries, guardSleepIntervals);
    

    return 0;
}