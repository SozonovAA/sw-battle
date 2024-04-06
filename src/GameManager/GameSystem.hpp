#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "IGameSystem.hpp"
#include "../Map/IMap.hpp"
#include "../Units/IUnit.hpp"

namespace sw::mngr {
    
    class GameSystem : public IGameSystem
    {
    public:
        using map_type = map::IMap<units::IUnit>;
        using units_storage_type = std::unordered_set<map_type::entity_storage_type>;
        
        explicit GameSystem(std::shared_ptr<map_type> map) : _map(std::move(map)) {};
        void execute(const cmd::IUnitCommand &cmd) override;
    
    private:
        
        const std::shared_ptr<map_type> _map;
        units_storage_type _units;
    };
    
} // namespace sw::mngr