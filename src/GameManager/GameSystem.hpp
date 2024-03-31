#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

#include "IGameSystem.hpp"
#include "../Map/IMap.hpp"
#include "Units/IUnit.hpp"
namespace sw::mngr {

class GameSystem : public IGameSystem
{
    public:
    using map_type = std::shared_ptr<map::IMap<units::IUnit>>;
    using units_storage_type = std::unordered_set<units::IUnit>;

    GameSystem(map_type map) : map_(map) {};
    void execute(const cmd::IUnitCommand& cmd);

    private:

    const map_type map_;
    units_storage_type units_;
};

} // namespace sw::mngr