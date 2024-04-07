#include "DefaultFunctions.hpp"

#include <memory>

namespace sw::units::templates {
using namespace mngr::cmd;
std::shared_ptr<IUnitCommand> DefaultMarchMethod(std::shared_ptr<IUnit> uRef)
{
    map::Point res;
    const unsigned stepCount = 1;
    const auto currPos = uRef->get_unit_position();
    const auto marchPos = uRef->get_march_position();
    const auto deltaPos = marchPos - currPos;

    if (std::abs(deltaPos._x) >= stepCount) {
        res._x = stepCount * (std::signbit(deltaPos._x) ? -1 : 1);
    }
    else {
        res._x = deltaPos._x;
    }
    if (std::abs(deltaPos._y) >= stepCount) {
        res._y = stepCount * (std::signbit(deltaPos._y) ? -1 : 1);
    }
    else {
        res._y = deltaPos._y;
    }
    return std::make_shared<UnitCommand<MoveDescription>>(uRef->get_id(), MoveDescription{res._x, res._y});
}

//todo: tests
std::shared_ptr<IUnit> GetAtckTarget(const std::vector<std::shared_ptr<IUnit>>& enemies)
{
    if(enemies.empty())
        return {};

    std::shared_ptr<IUnit> target = nullptr;
    for (const auto& enemy : enemies) 
    {
        if (!target || (enemy->get_hp() < target->get_hp()) || (enemy->get_id() < target->get_id())) 
            target = enemy;
    }

    return target;
}

std::shared_ptr<UnitCommand<MeleeAttackDescription>> MeleeAtcFunction(std::shared_ptr<IUnit> uRef, const std::shared_ptr<map::IMap<IUnit>> map)
{
    const auto unitPosition = uRef->get_unit_position();
    if( const auto &unitsAround = map->getUnitsAround(
        unitPosition._x,
        unitPosition._y,
        uRef->get_param_value("mRange")); !unitsAround.empty())
    {
        MeleeAttackDescription atcDescr {
                GetAtckTarget(unitsAround)->get_id(),
                uRef->get_param_value("strength")
        };
        return std::make_shared<UnitCommand<MeleeAttackDescription>>(uRef->get_id(), atcDescr);
    }
    return {};
}

std::shared_ptr<UnitCommand<RangeAttackDescription>> RangeAtcFunction(std::shared_ptr<IUnit> uRef, const std::shared_ptr<map::IMap<IUnit>> map)
{
    const auto unitPosition = uRef->get_unit_position();
    
    // что бы потом не фильтровать по расстоянию, разбиваем задачу на атомарные
    for(int toRange = 3, rRange = uRef->get_param_value("rRange"); toRange <= rRange; ++toRange)
    {
        if(const auto &unitsAround = map->getUnitsAround(
            unitPosition._x,
            unitPosition._y,
            toRange - 1,
            toRange); !unitsAround.empty())
        {
            RangeAttackDescription descr {
                    GetAtckTarget(unitsAround)->get_id(),
                    uRef->get_param_value("rRange"),
                    uRef->get_param_value("agility")
            };
            return std::make_shared<UnitCommand<RangeAttackDescription>>(uRef->get_id(), descr);
        }
    }
    return {};
}
}