#pragma once

#include "GameSystem.hpp"

#include "../IO/System/PrintDebug.hpp"
#include "../IO/System/EventLog.hpp"
#include "../IO/EventLogs/MapCreated.hpp"
#include "../IO/EventLogs/UnitSpawned.hpp"
#include "../IO/EventLogs/UnitMoved.hpp"
#include "../IO/EventLogs/UnitDied.hpp"
#include "../IO/EventLogs/UnitAttacked.hpp"
namespace sw::mngr {

class LoggableGameSystem : public GameSystem
{
    public:
    LoggableGameSystem(std::shared_ptr<map_type> map, const GameManager& gameManager);

    bool execute(const cmd::IUnitCommand &cmd) override;
    private:
    EventLog _eventLog;
};

}