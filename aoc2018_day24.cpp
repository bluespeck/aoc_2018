#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>

struct Group
{
    int64_t id;
    int64_t units;
    int64_t health;
    int64_t damage;
    int64_t initiative;
    std::string attackType;
    std::vector<std::string> weaknesses;
    std::vector<std::string> immunities;
    std::string faction;

    int64_t EffectivePower() const { return units * damage; }

    bool HasMoreEffectivePowerThan(const Group& other) const
    {
        return EffectivePower() > other.EffectivePower() ||
            (EffectivePower() == other.EffectivePower() && initiative > other.initiative);
    }

    bool HasImmunity(std::string test) const {
        return std::find(immunities.begin(), immunities.end(), test) != immunities.end();
    }

    bool HasWeakness(std::string test) const {
        return std::find(weaknesses.begin(), weaknesses.end(), test) != weaknesses.end();
    }

    int64_t PossibleDamage(const Group& target) const
    {
        int64_t possibleDamage = EffectivePower();

        if (target.HasImmunity(attackType))
            possibleDamage *= 0;
        else if (target.HasWeakness(attackType))
            possibleDamage *= 2;

        return possibleDamage;
    }
#pragma optimize ("",off)
    void TakeDamage(int64_t damage)
    {
        assert(damage > 0);
        units -= damage / health;
    }
#pragma optimize ("",on)
};

using GroupMap = std::unordered_map<int64_t, Group>;

using GroupVec = std::vector<int64_t>;
using AttackerToTargetMap = std::unordered_map<int64_t, std::pair<int64_t, int64_t>>;

int numFirstFaction = 0;

int TranslateGroupId(int groupId)
{
    return (groupId > numFirstFaction) ? groupId - numFirstFaction : groupId;
}

void PrintStatus(const GroupMap& groupMap)
{
    GroupVec immuneSystem, infection;
    for (auto keyVal : groupMap)
    {
        if (keyVal.second.faction == "Infection")
            infection.push_back(keyVal.first);
        else
            immuneSystem.push_back(keyVal.first);
    }
    std::sort(infection.begin(), infection.end());
    std::sort(immuneSystem.begin(), immuneSystem.end());

    std::cout << "Immune System:\n";
    for (auto id : immuneSystem)
    {
        std::cout << "Group " << TranslateGroupId(id) << " contains " << groupMap.at(id).units << "\n";
    }
    std::cout << "Infection:\n";
    for (auto id : infection)
    {
        std::cout << "Group " << TranslateGroupId(id) << " contains " << groupMap.at(id).units << "\n";
    }
    std::cout << "\n";
}

void PrintTargetChoices(const GroupMap& groupMap, const GroupVec& groups, GroupVec immuneSystem,GroupVec infection)
{
    std::set<int> viableTargetsFromImmuneSystem(immuneSystem.begin(), immuneSystem.end());
    std::set<int> viableTargetsFromInfection(infection.begin(), infection.end());

    for (auto group : groups)
    {
        auto& oppositeSide = groupMap.at(group).faction == "Infection" ? immuneSystem : infection;
        auto& oppositeSideViable = groupMap.at(group).faction == "Infection" ? viableTargetsFromImmuneSystem : viableTargetsFromInfection;
        std::sort(oppositeSide.begin(), oppositeSide.end(), [&groupMap, group](int64_t a, int64_t b) {
            auto damageA = groupMap.at(group).PossibleDamage(groupMap.at(a));
            auto damageB = groupMap.at(group).PossibleDamage(groupMap.at(b));
            return damageA > damageB;
        });
        for (auto opponent : oppositeSide)
        {
            if (oppositeSideViable.count(opponent) > 0)
            {
                auto damage = groupMap.at(group).PossibleDamage(groupMap.at(opponent));
                if (damage > 0)
                {
                    std::cout << groupMap.at(group).faction << " group " << TranslateGroupId(group) << " would deal defending group " << TranslateGroupId(opponent) << " " << damage << " damage" << "\n";
                    break;
                }
            }
        }
    }
}

AttackerToTargetMap ChooseTargets(GroupMap& groupMap)
{
    GroupVec groups, immuneSystem, infection;
    for (auto& keyVal : groupMap)
    {
        if (keyVal.second.faction == "Infection")
            infection.push_back(keyVal.first);
        else
            immuneSystem.push_back(keyVal.first);
        groups.push_back(keyVal.first);
    }

    std::sort(groups.begin(), groups.end(), [&groupMap](int64_t a, int64_t b) {
        return groupMap.at(a).HasMoreEffectivePowerThan(groupMap.at(b));
    });

    std::set<int> viableTargetsFromImmuneSystem(immuneSystem.begin(), immuneSystem.end());
    std::set<int> viableTargetsFromInfection(infection.begin(), infection.end());

    //PrintTargetChoices(groupMap, groups, immuneSystem, infection);

    AttackerToTargetMap att;
    for (auto group : groups)
    {
        auto& oppositeSide = groupMap.at(group).faction == "Infection" ? immuneSystem : infection;
        auto& oppositeSideViable = groupMap.at(group).faction == "Infection" ? viableTargetsFromImmuneSystem : viableTargetsFromInfection;
        std::sort(oppositeSide.begin(), oppositeSide.end(), [&groupMap, group](int64_t a, int64_t b) {
            auto damageA = groupMap.at(group).PossibleDamage(groupMap.at(a));
            auto damageB = groupMap.at(group).PossibleDamage(groupMap.at(b));
            return damageA > damageB || (damageA == damageB && groupMap.at(a).HasMoreEffectivePowerThan(groupMap.at(b)));
        });
        for (auto opponent : oppositeSide)
        {
            if (oppositeSideViable.count(opponent) > 0)
            {
                auto damage = groupMap.at(group).PossibleDamage(groupMap.at(opponent));
                if (damage > 0)
                {
                    att[group] = std::pair{ opponent, damage };
                    oppositeSideViable.erase(opponent);
                    break;
                }
            }
        }
    }
    return att;
}



void SimulateBattle(GroupMap& groupMap, AttackerToTargetMap& att)
{
    GroupVec groups, immuneSystem, infection;
    for (auto& keyVal : groupMap)
    {
        if (keyVal.second.faction == "Infection")
            infection.push_back(keyVal.first);
        else
            immuneSystem.push_back(keyVal.first);
        groups.push_back(keyVal.first);
    }

    std::sort(groups.begin(), groups.end(), [&groupMap](int64_t a, int64_t b)
    {
        return groupMap.at(a).initiative > groupMap.at(b).initiative;
    });

    //std::cout << "\n";
    for (auto group : groups)
    {
        if (groupMap.at(group).units <= 0 || att.find(group) == att.end() || groupMap.at(att.at(group).first).units <= 0)
            continue;

        const auto& attacker = groupMap.at(group);
        auto& defender = groupMap.at(att.at(group).first);
        //std::cout << attacker.faction << " group " << TranslateGroupId(attacker.id) << " attacks defending group " << TranslateGroupId(defender.id) << ", dealing " << attacker.PossibleDamage(defender) << " damage and killing " << std::min(defender.units, attacker.PossibleDamage(defender) / defender.health) << " units.\n";

        int64_t damage = attacker.PossibleDamage(defender);
        defender.TakeDamage(damage);
    }
    for (auto group : groups)
    {
        if (groupMap.at(group).units <= 0)
        {
            groupMap.erase(group);
        }
    }
}

GroupMap ReadInput(int &numFirstFaction)
{
    GroupMap groupMap;

    int64_t id = 1;
    std::string faction;
    std::getline(std::cin, faction);
    faction.erase(faction.end() - 1);
    while (std::cin)
    {
        std::string line;
        std::getline(std::cin, line);
        if (std::cin && line.size() == 0)
        {
            std::getline(std::cin, faction);
            faction.erase(faction.end() - 1);
            std::getline(std::cin, line);
            numFirstFaction = id - 1;
        }
        Group group;
        group.faction = faction;
        group.id = id;
        std::istringstream is(line);
        is >> group.units;
        is.ignore(std::string(" units each with ").length());
        is >> group.health;
        is.ignore(std::string(" hit points ").length());
        char c;
        is >> c;
        if (c == '(')
        {
            bool doOneMore = false;
            do
            {
                doOneMore = false;
                is >> c;
                if (c == 'i')
                {
                    is.ignore(std::string("mmune to ").length());
                    std::string attackType;
                    while (is.peek() != ';' && is.peek() != ')')
                    {
                        is >> attackType;
                        if (attackType.back() == ',' || attackType.back() == ')' || attackType.back() == ';')
                        {
                            group.immunities.push_back(attackType.substr(0, attackType.size() - 1));
                        }

                        if (attackType.back() == ')' || attackType.back() == ';')
                        {
                            is.putback(attackType.back());
                        }
                    }
                }
                else if (c == 'w')
                {
                    is.ignore(std::string("eak to ").length());
                    std::string attackType;
                    while (is.peek() != ';' && is.peek() != ')')
                    {
                        is >> attackType;
                        if (attackType.back() == ',' || attackType.back() == ')' || attackType.back() == ';')
                        {
                            group.weaknesses.push_back(attackType.substr(0, attackType.size() - 1));
                        }

                        if (attackType.back() == ')' || attackType.back() == ';')
                        {
                            is.putback(attackType.back());
                        }
                    }
                }
                if (is.get() == ';')
                {
                    doOneMore = true;
                }
            } while (doOneMore == true);
            is.ignore(1);
        }
        else
        {
            is.putback(c);
        }
        is.ignore(std::string("with an attack that does ").length());
        is >> group.damage >> group.attackType;
        is.ignore(std::string(" damage at initiative ").length());
        is >> group.initiative;

        if (std::cin)
            groupMap[group.id] = group;

        id++;
    }

    return groupMap;
}

int64_t CountUnits(GroupMap& groupMap, std::string faction)
{
    int64_t count = 0;
    for(auto& kv : groupMap)
    {
        if (kv.second.faction == faction)
            count += kv.second.units > 0 ? kv.second.units : 0;
    }
    return count;
}

void SimulateWar(GroupMap groupMap, int64_t& unitCountInfection, int64_t& unitCountImmuneSystem, int64_t boost = 0)
{
    if (boost > 0)
    {
        for (auto& keyVal : groupMap)
        {
            if (keyVal.second.faction == "Immune System")
            {
                keyVal.second.damage += boost;
            }
        }
    }

    int step = 0;
    do
    {
        //std::cout << "Step: "<< step << "\n";
        //PrintStatus(groupMap);

        auto att = ChooseTargets(groupMap);
        if (att.size() == 0)
        {
            unitCountImmuneSystem = 0;
            unitCountInfection = 0;
            break;
        }
        auto prevUnitCountInfection = CountUnits(groupMap, "Infection");
        auto prevUnitCountImmuneSystem = CountUnits(groupMap, "Immune System");
        SimulateBattle(groupMap, att);
        unitCountInfection = CountUnits(groupMap, "Infection");
        unitCountImmuneSystem = CountUnits(groupMap, "Immune System");
        if (unitCountInfection == prevUnitCountInfection && unitCountImmuneSystem == prevUnitCountImmuneSystem)
        {
            unitCountImmuneSystem = 0;
            unitCountInfection = 0;
        }
        step++;
    } while (unitCountInfection > 0 && unitCountImmuneSystem > 0);

    if (unitCountInfection > 0)
        std::cout << "Infection wins with " << unitCountInfection << " units.\n";
    if (unitCountImmuneSystem > 0)
        std::cout << "Immune wins with " << unitCountImmuneSystem << " units.\n";
}

int main()
{
    auto groupMap = ReadInput(numFirstFaction);

    for (int64_t boost = 0; boost < 100'000; boost += 1000)
    {
        int64_t unitCountImmuneSystem = 0;
        int64_t unitCountInfection = 0;
        SimulateWar(groupMap, unitCountInfection, unitCountImmuneSystem, boost);
        if (unitCountImmuneSystem > 0)
        {
            int64_t minBoost = boost - 1000;
            int64_t maxBoost = boost;
            while (minBoost < maxBoost)
            {
                int64_t unitCountImmuneSystem = 0;
                int64_t unitCountInfection = 0;
                int64_t newBoost = (maxBoost + minBoost) / 2;
                SimulateWar(groupMap, unitCountInfection, unitCountImmuneSystem, newBoost);
                if (unitCountImmuneSystem > 0)
                    maxBoost = newBoost - 1;
                else
                    minBoost = newBoost + 1;
            }
            break;
        }
    }

    return 0;

}