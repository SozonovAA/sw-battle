#include "GameSystem.hpp"
#include "Command/CmdDescriptions.hpp"

namespace sw::mngr {
    
    void GameSystem::execute(const cmd::IUnitCommand &cmd)
    {
        const auto &commandRes = cmd.execute();
        switch (commandRes.type_)
        {
            case cmd::CmdType::MOVE:
            {
                const auto &mv = commandRes.get_description<cmd::CmdType::MOVE>();
                //todo:func
                break;
            }
            case cmd::CmdType::M_ATCK:
            {
                const auto &mv = commandRes.get_description<cmd::CmdType::M_ATCK>();
                //todo:func
                break;
            }
                break;
            case cmd::CmdType::R_ATCK:
            {
                const auto &mv = commandRes.get_description<cmd::CmdType::R_ATCK>();
                //todo:func
                break;
            }
            case cmd::CmdType::UNDEF:
                break;
        }
    }
    
} //namespace sw::mngr
