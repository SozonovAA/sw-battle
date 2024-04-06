//
// Created by User on 4/6/24.
//

#pragma once
#include <queue>
#include "../Map/IMap.hpp"
#include "../Units/IUnit.hpp"
#include "GameSystem.hpp"
#include "../Units/UnitBuilder.hpp"
#include "../Units/Unit.hpp"

namespace sw::mngr {

class GameManager final
{
public:
    template<class GameSystemT>
    GameManager() : _gameSystem(std::make_unique<GameSystemT>(_map)) {};
    
    using map_type = std::shared_ptr<map::IMap<units::IUnit>>;
    using unit_type = map_type::element_type::entity_storage_type;
    using units_storage_type = std::map<units::IUnit::id_type, unit_type>;
    
    void SpawnUnit(const units::WarriorDescription& descr, const map::Point& coord);
    void SpawnUnit(const units::ArcherDescription& descr, const map::Point& coord);
    
    static unit_type GetMeleeAtckTarget(const std::vector<unit_type>& enemies);
    static unit_type GetRangeAtckTarget(const std::vector<unit_type>& enemies);

private:
    units_storage_type _unitsStorage;
    map_type _map;
    std::unique_ptr<IGameSystem> _gameSystem;
    std::queue<cmd::CmdDescription> _commandsQueue;
};


} // namespace sw::mngr

