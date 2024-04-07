//
// Created by User on 4/6/24.
//

#include "../Units/BuilderTemplates/DefaultUnitsBuilders.hpp"
#include "GameManager.hpp"
#include "../Map/Coords.hpp"
#include "Command/CmdDescriptions.hpp"
#include "../Units/UnitTypes.hpp"
#include <cmath>
#include <cstddef>
#include <cstdlib>
namespace sw::mngr {
using SpawnCommand =
        mngr::cmd::UnitCommand<mngr::cmd::SpawnDescription>;

using namespace units::templates;

SpawnCommand GenerateSpawnCommand(std::shared_ptr<units::IUnit> unit, const map::Point& coord)
{
    mngr::cmd::SpawnDescription spDescr {
            unit,
            coord
    };
    return { unit->get_id(), spDescr };
}

//todo: to spawn class and tests
void GameManager::SpawnUnit(const units::WarriorDescription& descr, const units::UnitDescription& uDescr, const map::Point& coord)
{
    _gameSystem->execute(
        GenerateSpawnCommand(WarriorBuilder(descr, _map).create_unit(uDescr),
        coord)
    );
}

void GameManager::SpawnUnit(const units::ArcherDescription& descr, const units::UnitDescription& uDescr,  const map::Point& coord)
{
    units::UnitBuilder<units::UnitClass::ARCH, units::Unit> builder;
    
    _gameSystem->execute(
        GenerateSpawnCommand(ArcherBuilder(descr, _map).create_unit(uDescr),
        coord)
    );
}
} // sw
