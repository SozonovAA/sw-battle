#pragma once
#include "../../GameManager/Command/IUnitCommand.hpp"
#include "../../Map/IMap.hpp"

#include "../IUnit.hpp"

#include <memory>
namespace sw::units::templates {

std::shared_ptr<mngr::cmd::IUnitCommand> DefaultMarchMethod(std::shared_ptr<IUnit> uRef);
//todo: tests
std::shared_ptr<IUnit> GetAtckTarget(const std::vector<std::shared_ptr<IUnit>>& enemies);
std::shared_ptr<mngr::cmd::UnitCommand<mngr::cmd::MeleeAttackDescription>> MeleeAtcFunction(std::shared_ptr<IUnit> uRef, const std::shared_ptr<map::IMap<IUnit>> map);
std::shared_ptr<mngr::cmd::UnitCommand<mngr::cmd::RangeAttackDescription>> RangeAtcFunction(std::shared_ptr<IUnit> uRef, const std::shared_ptr<map::IMap<IUnit>> map);

}