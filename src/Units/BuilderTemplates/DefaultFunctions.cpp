#include "DefaultFunctions.hpp"
#include "../UnitTypes.hpp"

#include <limits>
#include <memory>

namespace sw::units::templates {
using namespace mngr::cmd;
std::shared_ptr<IUnitCommand> defaultMarchMethod(std::shared_ptr<IUnit> uRef)
{
    map::Point res;
    const unsigned stepCount = uRef->getStepCount();
    const auto currPos = uRef->getUnitPosition();
    const auto marchOpt = uRef->getMarchPosition();
    if(!marchOpt.has_value()) 
        return {};
    
    const auto marchPos = marchOpt.value();
    const auto [deltaX, deltaY] = marchPos - currPos;

    if (std::abs(deltaX) >= stepCount) {
        res._x = stepCount * (deltaX < 0 ? -1 : 1);
    }
    else {
        res._x = deltaX;
    }
    if (std::abs(deltaY) >= stepCount) {
        res._y = stepCount * (deltaY < 0 ? -1 : 1);
    }
    else {
        res._y = deltaY;
    }
    return std::make_shared<UnitCommand<MoveDescription>>(uRef->getId(), MoveDescription{res._x, res._y});
}

std::shared_ptr<IUnit> getAtckTarget(const std::vector<std::shared_ptr<IUnit>>& enemies)
{
    if(enemies.empty())
        return {};

    std::shared_ptr<IUnit> target = nullptr;
    for (const auto& enemy : enemies) 
    {
        if (!target || (enemy->getHp() < target->getHp()) || (enemy->getId() < target->getId())) 
            target = enemy;
    }

    return target;
}

std::shared_ptr<UnitCommand<MeleeAttackDescription>> meleeAtcFunction(std::shared_ptr<IUnit> uRef, const std::shared_ptr<map::IMap<IUnit>> map)
{
    const auto [x, y] = uRef->getUnitPosition();
    if( const auto &unitsAround = map->getUnitsAround(
        x,
        y,
        uRef->getParamValue("mRange")); !unitsAround.empty())
    {
        MeleeAttackDescription atcDescr {
                getAtckTarget(unitsAround)->getId(),
                static_cast<hp_type>(uRef->getParamValue("strength"))
        };
        return std::make_shared<UnitCommand<MeleeAttackDescription>>(uRef->getId(), atcDescr);
    }
    return {};
}

std::shared_ptr<UnitCommand<RangeAttackDescription>> rangeAtcFunction(std::shared_ptr<IUnit> uRef, const std::shared_ptr<map::IMap<IUnit>> map)
{
    const auto [x, y] = uRef->getUnitPosition();
     if(const auto &unitsAround = map->getUnitsAround(
            x,
            y,
            2,
            uRef->getParamValue("rRange")); !unitsAround.empty())
        {
            double minDistance = std::numeric_limits<double>().max();
            std::vector<std::shared_ptr<IUnit>> closeUnits;
            for(const auto & unit : unitsAround)
            {
                const auto [tarX, tarY] = unit->getUnitPosition();
                if(const auto dist = map::getDistance( x, y, tarX, tarY); minDistance >= dist)
                    closeUnits.push_back(unit);
            }

            RangeAttackDescription descr {
                    getAtckTarget(closeUnits)->getId(),
                    uRef->getParamValue("rRange"),
                    static_cast<hp_type>(uRef->getParamValue("agility"))
            };
            return std::make_shared<UnitCommand<RangeAttackDescription>>(uRef->getId(), descr);
        }

    return {};
}
}