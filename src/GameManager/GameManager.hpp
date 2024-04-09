//
// Created by User on 4/6/24.
//

#pragma once
#include <memory>
#include <queue>
#include <sys/_types/_id_t.h>
#include <utility>
#include "../Map/IMap.hpp"
#include "../Units/IUnit.hpp"
#include "IGameSystem.hpp"
#include "../Units/UnitBuilder.hpp"
#include "../Units/BuilderTemplates/DefaultUnitsBuilders.hpp"
#include "../Units/Unit.hpp"
#include "../Units/UnitTypes.hpp"
#include "../Map/Coords.hpp"

namespace sw::mngr {

class GameManager final
{
public:
    using map_type = std::shared_ptr<map::IMap<units::IUnit>>;
    using unit_type = map_type::element_type::entity_storage_type;
    using units_storage_type = std::map<units::IUnit::id_type, unit_type>;

    GameManager(map_type map) :_map(map) {};

    template<class GameSystemT>
    std::shared_ptr<IGameSystem> CreateGameSystem()
    {
        return _gameSystem = std::make_shared<GameSystemT>(_map, *this);
    }
    
    template<class UnitType>
    std::shared_ptr<const units::IUnit> SpawnUnit(const units::WarriorDescription& descr, const units::UnitDescription& uDescr, const map::Point& coord)
    {
        units::templates::WarriorBuilder<UnitType> builder (descr, _map);
        auto unit = builder.create_unit(uDescr);
        SpawnProcess(unit, coord);
        return unit;
    }
    
    template<class UnitType>
    std::shared_ptr<const units::IUnit> SpawnUnit(const units::ArcherDescription& descr, const units::UnitDescription& uDescr, const map::Point& coord)
    {
        units::templates::ArcherBuilder<UnitType> builder (descr, _map);
        auto unit = builder.create_unit(uDescr);
        
        SpawnProcess(unit, coord);
        return unit;

    }

    unit_type GetUnitById(const units::id_type& id) const;
    void SetMarchForUnit(const units::id_type& id, const map::Point& marchAim);

    void WaitOneGameTick();
private:
    std::queue<std::pair<units::id_type, std::shared_ptr<cmd::IUnitCommand>>> CheckUnitsState();
    void SpawnProcess(std::shared_ptr<units::IUnit> unit, const map::Point& coord);
private:
    units_storage_type _unitsStorage;
    map_type _map;
    std::shared_ptr<IGameSystem> _gameSystem;
    std::queue<std::shared_ptr<cmd::IUnitCommand>> _commandsQueue;

};


} // namespace sw::mngr

