//
// Created by User on 4/6/24.
//

#pragma once
#include <limits>
#include <memory>
#include <queue>
#include <utility>
#include "../Map/IMap.hpp"
#include "../Units/IUnit.hpp"
#include "IGameExecutor.hpp"
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

    template<class MapT, class GameExecutorT>
    std::shared_ptr<MapT> create_map(const map::Point::coord_type& x, const map::Point::coord_type& y)
    {
        auto res =std::make_shared<MapT>(x, y);
        _map = res;
        _gameExecutor = std::make_shared<GameExecutorT>(_map, *this);
        return res;
    }
    
    template<class UnitType, class DescriptorType>
    std::shared_ptr<units::IUnit> SpawnUnit(const DescriptorType& descr, const units::UnitDescription& uDescr, const map::Point& coord)
    {
        auto unit = units::templates::CreateDefaultUnit<UnitType>(descr, _map, uDescr);
        SpawnProcess(unit, coord);
        return unit;
    }
    unit_type GetUnitById(const units::id_type& id) const;
    void SetMarchForUnit(const units::id_type& id, const map::Point& marchAim);
    void WaitOneGameTick();
    void WaitGameTicks(unsigned int n);
private:
    std::queue<std::pair<units::id_type, std::shared_ptr<cmd::IUnitCommand>>> CheckUnitsState();
    void SpawnProcess(std::shared_ptr<units::IUnit> unit, const map::Point& coord);
private:
    units_storage_type _unitsStorage;
    map_type _map;
    std::shared_ptr<IGameExecutor> _gameExecutor;
    std::queue<std::shared_ptr<cmd::IUnitCommand>> _commandsQueue;
};
class IExecutor
{
    public:
    bool execute();

};


} // namespace sw::mngr

