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
    WarriorBuilder(const WarriorDescription& descr, const std::shared_ptr<map::IMap<IUnit>>& map)
    {
        this->addParam("strength", descr.strength);
        this->addParam("mRange", descr.mRange);

        this->setMarchMethod(
            [](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<IUnitCommand>
            {
                return defaultMarchMethod(uRef);
            }
        );

        this->addActionByPriority(
            0,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<IUnitCommand>
            {
                return meleeAtcFunction(uRef, map);
            }
        );
    }
};

template<class UnitType>
static std::shared_ptr<units::IUnit> createDefaultUnit(const units::WarriorDescription& descr, const std::shared_ptr<map::IMap<IUnit>>& map, const units::UnitDescription& uDescr)
{
    units::templates::WarriorBuilder<UnitType> builder (descr, map);
    return builder.createUnit(uDescr);
}

template<class UnitType = Unit>
class ArcherBuilder : public UnitBuilder<UnitClass::ARCH, UnitType>
{
    public:
    using type = UnitBuilder<UnitClass::ARCH, UnitType>;
    ArcherBuilder(const ArcherDescription& descr, const std::shared_ptr<map::IMap<IUnit>>& map)
    {
        this->addParam("strength", descr.strength);
        this->addParam("agility", descr.agility);
        this->addParam("mRange", descr.mRange);
        this->addParam("rRange", descr.rRange);
        this->setMarchMethod(
            [](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return defaultMarchMethod(uRef);
            }
        );
        this->addActionByPriority(
            0,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return rangeAtcFunction(uRef, map);
            }
        );
        this->addActionByPriority(
            1,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return meleeAtcFunction(uRef, map);
            }
        );
    }
};

template<class UnitType>
static std::shared_ptr<units::IUnit> createDefaultUnit(const units::ArcherDescription& descr, const std::shared_ptr<map::IMap<IUnit>>& map, const units::UnitDescription& uDescr)
{
    units::templates::ArcherBuilder<UnitType> builder (descr, map);
    return builder.createUnit(uDescr);
}
} // namespace sw::units::templates 