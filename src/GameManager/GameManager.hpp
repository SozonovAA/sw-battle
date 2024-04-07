//
// Created by User on 4/6/24.
//

#pragma once
#include <memory>
#include <queue>
#include "../Map/IMap.hpp"
#include "../Units/IUnit.hpp"
#include "IGameSystem.hpp"
#include "../Units/UnitBuilder.hpp"
#include "../Units/Unit.hpp"
#include "../Units/UnitTypes.hpp"
#include "../Map/Coords.hpp"

namespace sw::mngr {

class GameManager final
{
public:
    template<class GameSystemT>
    GameManager() : _gameSystem(std::make_unique<GameSystemT>(_map)) {};
    
    using map_type = std::shared_ptr<map::IMap<units::IUnit>>;
    using unit_type = map_type::element_type::entity_storage_type;
    using units_storage_type = std::map<units::IUnit::id_type, unit_type>;
    
    void SpawnUnit(const units::WarriorDescription& descr, const units::UnitDescription& uDescr, const map::Point& coord);
    void SpawnUnit(const units::ArcherDescription& descr, const units::UnitDescription& uDescr, const map::Point& coord);

    unit_type GetUnitById(const units::id_type& id);
    void SetMarchForUnit(const units::id_type& id, const map::Point& marchAim);
private:
    void SpawnProcess(std::shared_ptr<units::IUnit> unit, const map::Point& coord);
private:
    units_storage_type _unitsStorage;
    map_type _map;
    std::unique_ptr<IGameSystem> _gameSystem;
    std::queue<cmd::CmdDescription> _commandsQueue;

};


} // namespace sw::mngr

