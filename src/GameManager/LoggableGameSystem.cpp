#include "LoggableGameSystem.hpp"
#include "CoreFunctions.hpp"

namespace sw::mngr {

using namespace units;
using namespace cmd;
using namespace map;

LoggableGameSystem::LoggableGameSystem(std::shared_ptr<map_type> map, const GameManager& gameManager) : 
    GameSystem(map, gameManager)
{
    _eventLog.listen<sw::io::MapCreated>([](auto& event){ printDebug(std::cout, event); });
    _eventLog.listen<sw::io::UnitSpawned>([](auto& event){ printDebug(std::cout, event); });
    _eventLog.listen<sw::io::UnitMoved>([](auto& event){ printDebug(std::cout, event); });
    _eventLog.listen<sw::io::UnitAttacked>([](auto& event){ printDebug(std::cout, event); });
    _eventLog.listen<sw::io::UnitDied>([](auto& event){ printDebug(std::cout, event); });
}

bool LoggableGameSystem::execute(const cmd::IUnitCommand &cmd)
{
    bool res = GameSystem::execute(cmd);
    const auto &commandRes = cmd.execute();
    switch (commandRes._type)
    {
        case cmd::CmdType::SKIP:
        {
            break;
        }
        case cmd::CmdType::SPAWN:
        {
            if(res)
            {
                const auto descr = commandRes.get_description<cmd::CmdType::SPAWN>();
                const auto unitPtr = std::any_cast<std::shared_ptr<IUnit>>(descr.unit);
                const uint32_t x = descr.coord._x;
                const uint32_t y = descr.coord._y;
                _eventLog.log(io::UnitSpawned{ unitPtr->get_id(), UnitTypeLogString.at(unitPtr->get_class()), x, y});
            }
            break;
        }
        case cmd::CmdType::DEAD:
        {
            if(res)
                _eventLog.log(io::UnitDied{ commandRes._id });

            break;
        }

        case cmd::CmdType::MOVE:
        {
            if(res)
            {
                const auto descr = commandRes.get_description<cmd::CmdType::MOVE>();

                auto unit = _gameManager.GetUnitById(commandRes._id);
                const auto coord = unit->get_unit_position();
                const uint32_t x = coord._x;
                const uint32_t y = coord._y;
                _eventLog.log(io::UnitMoved{ commandRes._id, x, y});
            }
            break;
        }
        case cmd::CmdType::M_ATCK:
        {
            if(res)
            {
                const auto descr = commandRes.get_description<cmd::CmdType::M_ATCK>();
                auto unit = _gameManager.GetUnitById(descr.unit_id);
                auto hp = unit->get_hp();
                _eventLog.log(io::UnitAttacked{ 
                    commandRes._id,
                    descr.unit_id, 
                    static_cast<uint32_t>(descr.damage), 
                    static_cast<uint32_t>(hp < 0 ? 0 : hp)
                });
                break;
            }
        }
        case cmd::CmdType::R_ATCK:
        {
            if(res)
            {
                const auto descr = commandRes.get_description<cmd::CmdType::R_ATCK>();
                auto unit = _gameManager.GetUnitById(descr.unit_id);
                auto hp = unit->get_hp();
                _eventLog.log(io::UnitAttacked{
                    commandRes._id,
                    descr.unit_id, 
                    static_cast<uint32_t>(descr.damage), 
                    static_cast<uint32_t>(hp < 0 ? 0 : hp)
                });
                break;
            }
        }
        case cmd::CmdType::UNDEF:
            throw std::runtime_error("Undefined command type!");
    }
    return res;
}
}