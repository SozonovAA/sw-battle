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
class WarriorBuilder : public UnitBuilder<UnitClass::WAR, Unit>
{
    public:
    using type = UnitBuilder<UnitClass::WAR, Unit>;
    WarriorBuilder(const WarriorDescription& descr, const std::shared_ptr<map::IMap<IUnit>> map)
    {
        add_param("strength", descr.strength);
        add_param("mRange", descr.mRange);

        set_march_method(
            [](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<IUnitCommand>
            {
                return DefaultMarchMethod(uRef);
            }
        );

        add_action_by_priority(
            0,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<IUnitCommand>
            {
                return MeleeAtcFunction(uRef, map);
            }
        );
    }
};

class ArcherBuilder : public UnitBuilder<UnitClass::WAR, Unit>
{
    public:
    using type = UnitBuilder<UnitClass::WAR, Unit>;
    ArcherBuilder(const ArcherDescription& descr, const std::shared_ptr<map::IMap<IUnit>> map)
    {
        add_param("strength", descr.strength);
        add_param("agility", descr.agility);
        add_param("mRange", descr.mRange);
        add_param("rRange", descr.rRange);
        set_march_method(
            [](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return DefaultMarchMethod(uRef);
            }
        );
        add_action_by_priority(
            0,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return RangeAtcFunction(uRef, map);
            }
        );
        add_action_by_priority(
            1,
            [map](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                return MeleeAtcFunction(uRef, map);
            }
        );
    }
};
} // namespace sw::units::templates 