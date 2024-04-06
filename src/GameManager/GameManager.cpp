//
// Created by User on 4/6/24.
//

#include "GameManager.hpp"
#include "../Map/Coords.hpp"
#include "Command/CmdDescriptions.hpp"
#include <cmath>
#include <cstddef>
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

//todo: to spawn class and tests
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
                if( const auto &unitsAround = map->getUnitsAround(
                    unitPosition._x,
                    unitPosition._y,
                    uRef.get_param_value("mRange")); !unitsAround.empty())
                {
                    MeleeAttackDescription atcDescr {
                            GameManager::GetAtckTarget(unitsAround)->get_id(),
                            uRef.get_param_value("strength")
                    };
                    return std::make_shared<UnitCommand<MeleeAttackDescription>>(uRef.get_id(), atcDescr);
                }
                return {};
                
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
            
            // что бы потом не фильтровать по расстоянию, разбиваем задачу на атомарные
            for(int toRange = 3, rRange = uRef.get_param_value("rRange"); toRange <= rRange; ++toRange)
            {
                if(const auto &unitsAround = map->getUnitsAround(
                    unitPosition._x,
                    unitPosition._y,
                    toRange - 1,
                    toRange); !unitsAround.empty())
                {
                    RangeAttackDescription descr {
                            GameManager::GetAtckTarget(unitsAround)->get_id(),
                            uRef.get_param_value("rRange"),
                            uRef.get_param_value("agility")
                    };
                    return std::make_shared<UnitCommand<RangeAttackDescription>>(uRef.get_id(), descr);
                }
            }
            return {};
        }
    );
    builder.add_action_by_priority(
        1,
        [map = _map](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
        {
            using namespace mngr::cmd;
            const auto unitPosition = uRef.get_unit_position();
            if( const auto &unitsAround = map->getUnitsAround(
                unitPosition._x,
                unitPosition._y,
                uRef.get_param_value("mRange")); !unitsAround.empty())
            {
                MeleeAttackDescription atcDescr {
                        GameManager::GetAtckTarget(unitsAround)->get_id(),
                        uRef.get_param_value("strength")
                };
                return std::make_shared<UnitCommand<MeleeAttackDescription>>(uRef.get_id(), atcDescr);
            }
            return {};
        }
    );
    
    _gameSystem->execute(GenerateSpawnCommand(builder.create_unit(descr.id, descr.hp), coord));
}

//todo: tests
GameManager::unit_type GameManager::GetAtckTarget(const std::vector<GameManager::unit_type>& enemies)
{
    if(enemies.empty())
        return {};

    GameManager::unit_type target = nullptr;
    for (const auto& enemy : enemies) 
    {
        if (!target || (enemy->get_hp() < target->get_hp()) || (enemy->get_id() < target->get_id())) 
            target = enemy;
    }

    return target;
}
} // sw
