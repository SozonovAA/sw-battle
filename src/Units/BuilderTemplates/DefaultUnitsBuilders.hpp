#pragma once
#include "../../GameManager/Command/IUnitCommand.hpp"
#include "../../Map/IMap.hpp"

#include "../UnitBuilder.hpp"
#include "../Unit.hpp"
#include "../IUnit.hpp"

#include "DefaultFunctions.hpp"

#include <memory>

namespace sw::units::templates {

using namespace mngr::cmd;

template<class UnitType = Unit>
class WarriorBuilder : public UnitBuilder<UnitClass::WAR, UnitType>
{
    public:
    using type = UnitBuilder<UnitClass::WAR, UnitType>;
    WarriorBuilder(const WarriorDescription& descr, const std::shared_ptr<map::IMap<IUnit>> map)
    {
        this->add_param("strength", descr.strength);
        this->add_param("mRange", descr.mRange);

        this->set_march_method(
            [](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<IUnitCommand>
            {
                return DefaultMarchMethod(uRef);
            }
        );

        this->add_action_by_priority(
            0,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<IUnitCommand>
            {
                return MeleeAtcFunction(uRef, map);
            }
        );
    }
};

template<class UnitType>
static std::shared_ptr<units::IUnit> CreateDefaultUnit(const units::WarriorDescription& descr, const std::shared_ptr<map::IMap<IUnit>> map, const units::UnitDescription& uDescr)
{
    units::templates::WarriorBuilder<UnitType> builder (descr, map);
    return builder.create_unit(uDescr);
}

template<class UnitType = Unit>
class ArcherBuilder : public UnitBuilder<UnitClass::ARCH, UnitType>
{
    public:
    using type = UnitBuilder<UnitClass::ARCH, UnitType>;
    ArcherBuilder(const ArcherDescription& descr, const std::shared_ptr<map::IMap<IUnit>> map)
    {
        this->add_param("strength", descr.strength);
        this->add_param("agility", descr.agility);
        this->add_param("mRange", descr.mRange);
        this->add_param("rRange", descr.rRange);
        this->set_march_method(
            [](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return DefaultMarchMethod(uRef);
            }
        );
        this->add_action_by_priority(
            0,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return RangeAtcFunction(uRef, map);
            }
        );
        this->add_action_by_priority(
            1,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return MeleeAtcFunction(uRef, map);
            }
        );
    }
};

template<class UnitType>
static std::shared_ptr<units::IUnit> CreateDefaultUnit(const units::ArcherDescription& descr, const std::shared_ptr<map::IMap<IUnit>> map, const units::UnitDescription& uDescr)
{
    units::templates::ArcherBuilder<UnitType> builder (descr, map);
    return builder.create_unit(uDescr);
}
} // namespace sw::units::templates 