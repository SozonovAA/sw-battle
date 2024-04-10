#pragma once

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../../IGameExecutor.hpp"
#include "../../GameManager.hpp"
#include "../../../Map/IMap.hpp"
#include "../../../Units/IUnit.hpp"

namespace sw::mngr {

class GameExecutor : public IGameExecutor
{
public:
    using map_type = map::IMap<units::IUnit>;
    using units_storage_type = std::unordered_set<map_type::entity_storage_type>;
    using handler_type = std::unordered_map<
        cmd::CmdType, 
        std::function<bool(const cmd::CmdDescription&, GameExecutor&)>
    >;
    
    GameExecutor(std::shared_ptr<map_type> map, const GameManager& gameManager);
    bool execute(const cmd::IUnitCommand &cmd) override;

    const GameManager& getGameManager()
    {
        return _gameManager;
    }

    std::shared_ptr<map_type> getMap()
    {
        return _map;
    }

protected:
    const GameManager& _gameManager;
    std::shared_ptr<map_type> _map;
private:
    static std::unordered_map<
        cmd::CmdType, 
        std::function<bool(const cmd::CmdDescription&, GameExecutor&)>
    > game_handlers;
};


    
} // namespace sw::mngr