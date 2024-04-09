#include "GameSystem.hpp"
#include "Command/CmdDescriptions.hpp"
#include "CoreFunctions.hpp"
#include <any>
#include <iostream>
#include <memory>

namespace sw::mngr {
using namespace units;
using namespace cmd;
using namespace map;

GameSystem::GameSystem(std::shared_ptr<map_type> map, const GameManager& gameManager) :
    _map(map),
    _gameManager(gameManager)
    {
        if (_map == nullptr)
            throw std::runtime_error("Undefined map!");
        
    };


bool GameSystem::execute(const cmd::IUnitCommand &cmd)
{
    bool res = false;
    const auto &commandRes = cmd.execute();
    switch (commandRes._type)
    {
        case cmd::CmdType::SKIP:
        {
            break;
        }
        case cmd::CmdType::SPAWN:
        {
            const auto descr = commandRes.get_description<cmd::CmdType::SPAWN>();
            try {
                const auto unitPtr = std::any_cast<std::shared_ptr<IUnit>>(descr.unit);
                if(_map->addUnit(descr.coord._x, descr.coord._y, unitPtr) == 0)
                {
                    unitPtr->set_unit_position({descr.coord._x, descr.coord._y});
                    res = true;
                }
            } catch (std::bad_any_cast& ex)
            {
                res = false;
            }
            break;
        }
        case cmd::CmdType::DEAD:
        {
            const auto descr = commandRes.get_description<cmd::CmdType::DEAD>();
            
            if(auto unit = _gameManager.GetUnitById(commandRes._id))
            {
                const auto coord = unit->get_unit_position();
                _map->deleteUnit(coord._x, coord._y);
                res = true;
            }
            break;
        }

        case cmd::CmdType::MOVE:
        {
            const auto descr = commandRes.get_description<cmd::CmdType::MOVE>();
            if(auto unit = _gameManager.GetUnitById(commandRes._id))
                res = UpdateUnitPosition(unit, descr, _map);

            break;
        }
        case cmd::CmdType::M_ATCK:
        {
            const auto descr = commandRes.get_description<cmd::CmdType::M_ATCK>();
            if(auto unit = _gameManager.GetUnitById(descr.unit_id))
            {
                unit->change_hp(-descr.damage);
                res = true;
            }
            break;
        }
        case cmd::CmdType::R_ATCK:
        {
            const auto descr = commandRes.get_description<cmd::CmdType::R_ATCK>();
                if(auto unit = _gameManager.GetUnitById(descr.unit_id))
            {
                unit->change_hp(-descr.damage);
                res = true;
            }
            break;
        }
        case cmd::CmdType::UNDEF:
            throw std::runtime_error("Undefined command type!");

}
    return res;
}

} //namespace sw::mngr
