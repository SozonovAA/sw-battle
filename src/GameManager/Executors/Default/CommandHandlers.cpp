
#include "GameExecutor.hpp"
#include "../CoreFunctions.hpp"

namespace sw::mngr {
namespace {


bool skip_handler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    return true;
}

bool spawn_handler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};
    const auto descr = commandRes.get_description<cmd::CmdType::SPAWN>();
    try {
        const auto unitPtr = std::any_cast<std::shared_ptr<units::IUnit>>(descr.unit);
        if(unitPtr && gs.get_map()->addUnit(descr.coord._x, descr.coord._y, unitPtr) == 0)
        {
            unitPtr->set_unit_position({descr.coord._x, descr.coord._y});
            res = true;
        }
    } catch (std::bad_any_cast& ex)
    {
        res = false;
    }
    return res;
}

bool dead_handler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};
    const auto descr = commandRes.get_description<cmd::CmdType::DEAD>();
    
    if(auto unit = gs.get_game_manager().GetUnitById(commandRes._id))
    {
        const auto coord = unit->get_unit_position();
        gs.get_map()->deleteUnit(coord._x, coord._y);
        res = true;
    }
    return res;
}

bool move_handler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};

    const auto descr = commandRes.get_description<cmd::CmdType::MOVE>();
    if(auto unit = gs.get_game_manager().GetUnitById(commandRes._id))
        res = UpdateUnitPosition(unit, descr, gs.get_map());

    return res;
}

bool mAtck_handler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};

    const auto descr = commandRes.get_description<cmd::CmdType::M_ATCK>();
    if(auto unit = gs.get_game_manager().GetUnitById(descr.unit_id))
    {
        unit->change_hp(-descr.damage);
        res = true;
    }
    return res;
}

bool rAtck_handler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};

    const auto descr = commandRes.get_description<cmd::CmdType::R_ATCK>();
        if(auto unit = gs.get_game_manager().GetUnitById(descr.unit_id))
    {
        unit->change_hp(-descr.damage);
        res = true;
    }
    return res;
}

bool undef_handler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    throw std::runtime_error("Undefined command type!");
}
}

GameExecutor::handler_type GameExecutor::game_handlers = 
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