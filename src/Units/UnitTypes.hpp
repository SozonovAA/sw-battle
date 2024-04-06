#pragma once
namespace sw::units {

using param_type = unsigned;
using id_type = unsigned;
using hp_type = int;

enum class UnitClass
{
    UNDEF = 0,
    WAR,
    ARCH
};

struct UnitDescriptionBase
{
    hp_type hp;
    id_type id;
};

struct WarriorDescription : UnitDescriptionBase
{
    param_type strength;
    param_type mRange;
};

struct ArcherDescription : WarriorDescription
{
    param_type agility;
    param_type rRange;
};


} // namespace sw::units
