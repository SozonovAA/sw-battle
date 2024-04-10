#pragma once

#include "../Default/GameExecutor.hpp"

#include "../../../IO/System/PrintDebug.hpp"
#include "../../../IO/System/EventLog.hpp"
#include "../../../IO/EventLogs/MapCreated.hpp"
#include "../../../IO/EventLogs/UnitSpawned.hpp"
#include "../../../IO/EventLogs/UnitMoved.hpp"
#include "../../../IO/EventLogs/UnitDied.hpp"
#include "../../../IO/EventLogs/UnitAttacked.hpp"
namespace sw::mngr {

class LoggableGameExecutor : public GameExecutor
{
    public:
    using handler_type = std::unordered_map<
        cmd::CmdType, 
        std::function<void(const cmd::CmdDescription&, LoggableGameExecutor&, EventLog&, bool)>
    >;
    LoggableGameExecutor(std::shared_ptr<map_type> map, const GameManager& gameManager);

    bool execute(const cmd::IUnitCommand &cmd) override;
    private:
    EventLog _eventLog;
    static handler_type game_handlers;
};

}