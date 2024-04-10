#include "LoggableGameExecutor.hpp"

namespace sw::mngr {

using namespace units;
using namespace cmd;
using namespace map;

LoggableGameExecutor::LoggableGameExecutor(std::shared_ptr<map_type> map, const GameManager& gameManager) : 
    GameExecutor(map, gameManager)
{
    _eventLog.listen<sw::io::MapCreated>([](auto& event){ printDebug(std::cout, event); });
    _eventLog.listen<sw::io::UnitSpawned>([](auto& event){ printDebug(std::cout, event); });
    _eventLog.listen<sw::io::UnitMoved>([](auto& event){ printDebug(std::cout, event); });
    _eventLog.listen<sw::io::UnitAttacked>([](auto& event){ printDebug(std::cout, event); });
    _eventLog.listen<sw::io::UnitDied>([](auto& event){ printDebug(std::cout, event); });
}

bool LoggableGameExecutor::execute(const cmd::IUnitCommand &cmd)
{
    bool res = GameExecutor::execute(cmd);
    const auto &commandRes = cmd.execute();
    LoggableGameExecutor::game_handlers[commandRes._type](commandRes, *this, _eventLog, res);
    return res;
}
}