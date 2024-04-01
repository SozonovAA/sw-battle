#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

#include "IGameSystem.hpp"
#include "../Map/IMap.hpp"
#include "../Units/IUnit.hpp"
namespace sw::mngr {

class GameSystem : public IGameSystem
{
    public:
    using map_type = map::IMap<units::IUnit>;
    using units_storage_type = std::unordered_set<map_type::entity_storage_type>;

    GameSystem(std::shared_ptr<map_type> map) : map_(map) {};
    void execute(const cmd::IUnitCommand& cmd);

    private:

    const std::shared_ptr<map_type> map_;
    units_storage_type units_;
};

} // namespace sw::mngr