//
// Created by User on 4/6/24.
//

#include "GameManager.hpp"
#include "../Map/Coords.hpp"
#include "Command/CmdDescriptions.hpp"
#include <cmath>
#include <cstdlib>
namespace sw::mngr {
using SpawnCommand =
        mngr::cmd::UnitCommand<mngr::cmd::SpawnDescription>;

SpawnCommand GenerateSpawnCommand(std::unique_ptr<units::IUnit> unit, const map::Point& coord)
{
    mngr::cmd::SpawnDescription spDescr {
            std::shared_ptr<units::IUnit> { unit.get() },
            coord
    };
    return { unit->get_id(), spDescr };
}

void GameManager::SpawnUnit(const units::WarriorDescription& descr, const map::Point& coord)
{
    units::UnitBuilder<units::UnitClass::WAR, units::Unit> builder;
    builder.add_param("strength", descr.strength);
    builder.add_param("mRange", descr.mRange);

    builder.set_march_method(
        [](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
        {
            return units::DefaultMarchMethod(uRef);
        }
    );

    builder.add_action_by_priority(
            0,
            [map = _map](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                using namespace mngr::cmd;
                const auto unitPosition = uRef.get_unit_position();
                const auto &unitsAround = map->getUnitsAround(
                        unitPosition._x,
                        unitPosition._y,
                        uRef.get_param_value("mRange"));
                
                MeleeAttackDescription atcDescr {
                        GameManager::GetMeleeAtckTarget(unitsAround)->get_id(),
                        uRef.get_param_value("strength")
                };
                return std::make_shared<UnitCommand<MeleeAttackDescription>>(uRef.get_id(), atcDescr);
            }
    );
    _gameSystem->execute(GenerateSpawnCommand(builder.create_unit(descr.id, descr.hp), coord));
}

void GameManager::SpawnUnit(const units::ArcherDescription& descr, const map::Point& coord)
{
    units::UnitBuilder<units::UnitClass::ARCH, units::Unit> builder;
    builder.add_param("strength", descr.strength);
    builder.add_param("agility", descr.agility);
    builder.add_param("mRange", descr.mRange);
    builder.add_param("rRange", descr.rRange);
    builder.set_march_method(
        [](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
        {
            return units::DefaultMarchMethod(uRef);
        }
    );
    builder.add_action_by_priority(
            0,
            [map = _map](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                using namespace mngr::cmd;
                const auto unitPosition = uRef.get_unit_position();
                const auto &unitsAround = map->getUnitsAround(
                    unitPosition._x,
                    unitPosition._y,
                    2,
                    uRef.get_param_value("rRange"));
                
                RangeAttackDescription descr {
                        GameManager::GetRangeAtckTarget(unitsAround)->get_id(),
                        uRef.get_param_value("rRange"),
                        uRef.get_param_value("agility")
                };
                return std::make_shared<UnitCommand<RangeAttackDescription>>(uRef.get_id(), descr);
            }
    );
    builder.add_action_by_priority(
            1,
            [map = _map](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                using namespace mngr::cmd;
                const auto unitPosition = uRef.get_unit_position();
                const auto &unitsAround = map->getUnitsAround(
                        unitPosition._x,
                        unitPosition._y,
                        uRef.get_param_value("mRange"));
                
                MeleeAttackDescription atcDescr {
                        GameManager::GetMeleeAtckTarget(unitsAround)->get_id(),
                        uRef.get_param_value("strength")
                };
                return std::make_shared<UnitCommand<MeleeAttackDescription>>(uRef.get_id(), atcDescr);
            }
    );
    
    _gameSystem->execute(GenerateSpawnCommand(builder.create_unit(descr.id, descr.hp), coord));
}

//todo: implement and tests
GameManager::unit_type GameManager::GetMeleeAtckTarget(const std::vector<GameManager::unit_type>& enemies)
{

}
GameManager::unit_type GameManager::GetRangeAtckTarget(const std::vector<GameManager::unit_type>& enemies)
{

}

} // sw
