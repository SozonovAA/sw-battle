
#include "LoggableGameExecutor.hpp"

namespace sw::mngr {
namespace {
void skipHandler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
}

void spawnHandler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
    {
        const auto descr = commandRes.getDescription<cmd::CmdType::SPAWN>();
        const auto unitPtr = std::any_cast<std::shared_ptr<units::IUnit>>(descr.unit);
        const uint32_t x = descr.coord.x;
        const uint32_t y = descr.coord.y;
        el.log(io::UnitSpawned{ unitPtr->getId(), units::UnitTypeLogString.at(unitPtr->getClass()), x, y});
    }
}

void deadHandler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
        el.log(io::UnitDied{ commandRes._id });
}

void moveHandler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
    {
        const auto descr = commandRes.getDescription<cmd::CmdType::MOVE>();

        auto unit = ge.getGameManager().getUnitById(commandRes._id);
        const auto coord = unit->getUnitPosition();
        const uint32_t x = coord.x;
        const uint32_t y = coord.y;
        el.log(io::UnitMoved{ commandRes._id, x, y});
    }
}

void mAtckHandler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
    {
        const auto descr = commandRes.getDescription<cmd::CmdType::M_ATCK>();
        auto unit = ge.getGameManager().getUnitById(descr.unit_id);
        auto hp = unit->getHp();
        el.log(io::UnitAttacked{ 
            commandRes._id,
            descr.unit_id, 
            static_cast<uint32_t>(descr.damage), 
            static_cast<uint32_t>(hp < 0 ? 0 : hp)
        });
    }
}

void rAtckHandler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
    {
        const auto descr = commandRes.getDescription<cmd::CmdType::R_ATCK>();
        auto unit = ge.getGameManager().getUnitById(descr.unit_id);
        auto hp = unit->getHp();
        el.log(io::UnitAttacked{
            commandRes._id,
            descr.unit_id, 
            static_cast<uint32_t>(descr.damage), 
            static_cast<uint32_t>(hp < 0 ? 0 : hp)
        });
    }
}

void undefHandler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
}
} // namespace 

LoggableGameExecutor::handler_type LoggableGameExecutor::game_handlers = 
{
    {cmd::CmdType::SPAWN, spawnHandler},
    {cmd::CmdType::SKIP, skipHandler},
    {cmd::CmdType::DEAD, deadHandler},
    {cmd::CmdType::MOVE, moveHandler},
    {cmd::CmdType::M_ATCK, mAtckHandler},
    {cmd::CmdType::R_ATCK, rAtckHandler},
    {cmd::CmdType::UNDEF, undefHandler}
};

}