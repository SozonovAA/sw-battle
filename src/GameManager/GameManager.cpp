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
#include <stdexcept>
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

void GameManager::SpawnProcess(std::shared_ptr<units::IUnit> unit, const map::Point& coord)
{
    if(!_gameSystem)
        throw std::runtime_error{"The GameSystem is not created!"};

    if(_gameSystem->execute(GenerateSpawnCommand(unit, coord)))
    {
        if(!_unitsStorage.emplace(unit->get_id(), unit).second)
            throw std::runtime_error{"A unit with the same ID has already spawned!"};
    }
    else 
        throw std::runtime_error{"The error of the spawn unit!"};
}

std::shared_ptr<const units::IUnit> GameManager::SpawnUnit(const units::WarriorDescription& descr, const units::UnitDescription& uDescr, const map::Point& coord)
{
    auto unit = WarriorBuilder(descr, _map).create_unit(uDescr);
    SpawnProcess(unit, coord);
    return unit;
}

std::shared_ptr<const units::IUnit> GameManager::SpawnUnit(const units::ArcherDescription& descr, const units::UnitDescription& uDescr,  const map::Point& coord)
{
    auto unit = ArcherBuilder(descr, _map).create_unit(uDescr);
    SpawnProcess(unit, coord);
    return unit;
}

GameManager::unit_type GameManager::GetUnitById(const units::id_type& id) const
{
    unit_type res {};
    if(auto it = _unitsStorage.find(id); it != _unitsStorage.end())
        res = it->second;

    return res;
}
void GameManager::SetMarchForUnit(const units::id_type& id, const map::Point& marchAim)
{
    if(auto it = _unitsStorage.find(id); it != _unitsStorage.end())
        it->second->set_march_position(marchAim);
    else 
        throw std::runtime_error{"Unknown ID!"};
}

std::queue<std::pair<units::id_type, std::shared_ptr<cmd::IUnitCommand>>> GameManager::CheckUnitsState()
{
    std::queue<std::pair<units::id_type, std::shared_ptr<cmd::IUnitCommand>>> res;
    for(const auto& unitPair : _unitsStorage)
    {
        if(const auto unit = unitPair.second; unit->get_state() == units::UnitState::DEAD)
            res.push({
            unitPair.first,
            std::make_shared<UnitCommand<DeadDescription>>(unit->get_id(), DeadDescription{})
            });
    }
    return res;
}

void GameManager::WaitOneGameTick()
{
    for(auto& unitPair : _unitsStorage)
        _commandsQueue.push(unitPair.second->process());

    while (!_commandsQueue.empty()) {
        _gameSystem->execute(*_commandsQueue.front());
        _commandsQueue.pop();
    }


    auto afterTurnCommandQueue = CheckUnitsState();
    while (!afterTurnCommandQueue.empty()) {
        if(const auto& front = afterTurnCommandQueue.front(); _gameSystem->execute(*front.second))
        {
            _unitsStorage.erase(front.first);
        }
        afterTurnCommandQueue.pop();
    }
}

} // sw
