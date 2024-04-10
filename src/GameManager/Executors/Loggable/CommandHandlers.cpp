
#include "LoggableGameExecutor.hpp"

namespace sw::mngr {
namespace {
void skip_handler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
}

void spawn_handler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
    {
        const auto descr = commandRes.get_description<cmd::CmdType::SPAWN>();
        const auto unitPtr = std::any_cast<std::shared_ptr<units::IUnit>>(descr.unit);
        const uint32_t x = descr.coord._x;
        const uint32_t y = descr.coord._y;
        el.log(io::UnitSpawned{ unitPtr->get_id(), units::UnitTypeLogString.at(unitPtr->get_class()), x, y});
    }
}

void dead_handler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
        el.log(io::UnitDied{ commandRes._id });
}

void move_handler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
    {
        const auto descr = commandRes.get_description<cmd::CmdType::MOVE>();

        auto unit = ge.get_game_manager().GetUnitById(commandRes._id);
        const auto coord = unit->get_unit_position();
        const uint32_t x = coord._x;
        const uint32_t y = coord._y;
        el.log(io::UnitMoved{ commandRes._id, x, y});
    }
}

void mAtck_handler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
    {
        const auto descr = commandRes.get_description<cmd::CmdType::M_ATCK>();
        auto unit = ge.get_game_manager().GetUnitById(descr.unit_id);
        auto hp = unit->get_hp();
        el.log(io::UnitAttacked{ 
            commandRes._id,
            descr.unit_id, 
            static_cast<uint32_t>(descr.damage), 
            static_cast<uint32_t>(hp < 0 ? 0 : hp)
        });
    }
}

void rAtck_handler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
    if(res)
    {
        const auto descr = commandRes.get_description<cmd::CmdType::R_ATCK>();
        auto unit = ge.get_game_manager().GetUnitById(descr.unit_id);
        auto hp = unit->get_hp();
        el.log(io::UnitAttacked{
            commandRes._id,
            descr.unit_id, 
            static_cast<uint32_t>(descr.damage), 
            static_cast<uint32_t>(hp < 0 ? 0 : hp)
        });
    }
}

void undef_handler(const cmd::CmdDescription& commandRes, LoggableGameExecutor& ge, EventLog& el, bool res)
{
}
}

LoggableGameExecutor::handler_type LoggableGameExecutor::game_handlers = 
{
    {cmd::CmdType::SPAWN, spawn_handler},
    {cmd::CmdType::SKIP, skip_handler},
    {cmd::CmdType::DEAD, dead_handler},
    {cmd::CmdType::MOVE, move_handler},
    {cmd::CmdType::M_ATCK, mAtck_handler},
    {cmd::CmdType::R_ATCK, rAtck_handler},
    {cmd::CmdType::UNDEF, undef_handler}
};

}