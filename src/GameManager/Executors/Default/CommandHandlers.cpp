
#include "GameExecutor.hpp"
#include "../CoreFunctions.hpp"

namespace sw::mngr {
namespace {

bool skipHandler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    return true;
}

bool spawnHandler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};
    const auto descr = commandRes.getDescription<cmd::CmdType::SPAWN>();
    try {
        const auto unitPtr = std::any_cast<std::shared_ptr<units::IUnit>>(descr.unit);
        if(unitPtr && gs.getMap()->addUnit(descr.coord._x, descr.coord._y, unitPtr) == 0)
        {
            unitPtr->setUnitPosition({descr.coord._x, descr.coord._y});
            res = true;
        }
    } catch (std::bad_any_cast& ex)
    {
        res = false;
    }
    return res;
}

bool deadHandler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};
    const auto descr = commandRes.getDescription<cmd::CmdType::DEAD>();
    
    if(auto unit = gs.getGameManager().getUnitById(commandRes._id))
    {
        const auto [x, y] = unit->getUnitPosition();
        gs.getMap()->deleteUnit(x, y);
        res = true;
    }
    return res;
}

bool moveHandler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};

    const auto descr = commandRes.getDescription<cmd::CmdType::MOVE>();
    if(auto unit = gs.getGameManager().getUnitById(commandRes._id))
        res = updateUnitPosition(unit, descr, gs.getMap());

    return res;
}

bool mAtckHandler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};

    const auto descr = commandRes.getDescription<cmd::CmdType::M_ATCK>();
    if(auto unit = gs.getGameManager().getUnitById(descr.unit_id))
    {
        unit->changeHp(-descr.damage);
        res = true;
    }
    return res;
}

bool rAtckHandler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    bool res{false};

    const auto descr = commandRes.getDescription<cmd::CmdType::R_ATCK>();
        if(auto unit = gs.getGameManager().getUnitById(descr.unit_id))
    {
        unit->changeHp(-descr.damage);
        res = true;
    }
    return res;
}

bool undefHandler(const cmd::CmdDescription& commandRes, GameExecutor& gs)
{
    throw std::runtime_error("Undefined command type!");
}
}

GameExecutor::handler_type GameExecutor::game_handlers = 
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