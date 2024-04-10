#include "GameExecutor.hpp"
#include "../CoreFunctions.hpp"
#include <any>
#include <iostream>
#include <memory>

namespace sw::mngr {
using namespace units;
using namespace cmd;
using namespace map;

GameExecutor::GameExecutor(std::shared_ptr<map_type> map, const GameManager& gameManager) :
    _map(map),
    _gameManager(gameManager)
    {
        if (_map == nullptr)
            throw std::runtime_error("Undefined map!");
        
    };

bool GameExecutor::execute(const cmd::IUnitCommand &cmd)
{
    const auto &commandRes = cmd.execute();
    return game_handlers[commandRes._type](commandRes, *this);
}

} //namespace sw::mngr
