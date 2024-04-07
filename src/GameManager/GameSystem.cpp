#include "GameSystem.hpp"
#include "Command/CmdDescriptions.hpp"
#include <any>
#include <iostream>
#include <memory>

namespace sw::mngr {
using namespace units;
using namespace cmd;
using namespace map;

bool UpdateUnitPosition(std::shared_ptr<IUnit> unit, MoveDescription expect, std::shared_ptr<IMap<IUnit>> map)
{
    bool res = false;
    auto uPos1 = unit->get_unit_position();
    auto newUPos1 = uPos1 + Point{expect.delta_x, expect.delta_y};
    if(!map->moveUnit(uPos1._x, uPos1._y, newUPos1._x, newUPos1._y))
    {
        unit->set_unit_position( newUPos1 );
        res = true;
    }

    return res;
}

GameSystem::GameSystem(std::shared_ptr<map_type> map, const GameManager& gameManager) :
    _map(map),
    _gameManager(gameManager)
    {};


bool GameSystem::execute(const cmd::IUnitCommand &cmd)
{
    bool res = false;
    const auto &commandRes = cmd.execute();
    switch (commandRes._type)
    {
        case cmd::CmdType::SKIP:
        {
            std::cout << "SKIP" << std::endl;
            //todo:func
            break;
        }
        case cmd::CmdType::SPAWN:
        {
            std::cout << "SPAWN" << std::endl;
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
            std::cout << "DEAD" << std::endl;
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
            std::cout << "MOVE" << std::endl;
            const auto descr = commandRes.get_description<cmd::CmdType::MOVE>();
            if(auto unit = _gameManager.GetUnitById(commandRes._id))
                res = UpdateUnitPosition(unit, descr, _map);

            break;
        }
        case cmd::CmdType::M_ATCK:
        {
            std::cout << "M_ATCK" << std::endl;
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
            std::cout << "R_ATCK" << std::endl;
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

break;
}

    return res;
}
    
} //namespace sw::mngr
