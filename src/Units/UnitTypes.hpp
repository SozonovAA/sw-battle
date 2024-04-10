#pragma once
#include <string>
#include <unordered_map>
namespace sw::units {

using param_type = unsigned;
using id_type = unsigned;
using hp_type = int;

enum class UnitClass
{
    UNDEF = 0,
    WAR,
    ARCH,
    MAG
};

const static std::unordered_map<UnitClass, std::string> UnitTypeLogString
{
    {UnitClass::WAR, "Warrior"},
    {UnitClass::ARCH, "Archer"},
    {UnitClass::MAG, "Mage"}
};

struct UnitDescription
{
    id_type id = 0;
    hp_type hp = 0;
    param_type step_count = 1;
};

struct WarriorDescription
{
    param_type strength;
    param_type mRange;
};

struct ArcherDescription : WarriorDescription
{
    param_type agility;
    param_type rRange;
};


struct MagDescription 
{
    param_type mana;
    param_type castRange;
};


} // namespace sw::units
