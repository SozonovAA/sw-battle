#pragma once
#include "../../GameManager/Command/IUnitCommand.hpp"
#include "../../Map/IMap.hpp"

#include "../IUnit.hpp"

#include <memory>
namespace sw::units::templates {

std::shared_ptr<mngr::cmd::IUnitCommand> defaultMarchMethod(std::shared_ptr<IUnit> uRef);
std::shared_ptr<IUnit> getAtckTarget(const std::vector<std::shared_ptr<IUnit>>& enemies);
std::shared_ptr<mngr::cmd::UnitCommand<mngr::cmd::MeleeAttackDescription>> meleeAtcFunction(std::shared_ptr<IUnit> uRef, const std::shared_ptr<map::IMap<IUnit>>& map);
std::shared_ptr<mngr::cmd::UnitCommand<mngr::cmd::RangeAttackDescription>> rangeAtcFunction(std::shared_ptr<IUnit> uRef, const std::shared_ptr<map::IMap<IUnit>>& map);

}