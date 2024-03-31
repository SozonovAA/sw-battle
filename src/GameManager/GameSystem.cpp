#include "GameSystem.hpp"
#include "GameManager/Command/CmdDescriptions.hpp"
#include "GameManager/Command/Impls/RangeAttackCommand.hpp"

namespace sw::mngr {

    void GameSystem::execute(const cmd::IUnitCommand& cmd)
    {
        const auto & commandRes = cmd.execute();
        switch (commandRes.type_) 
        {
        case cmd::CmdType::MOVE:
        {
            auto mv = commandRes.get_description<cmd::MoveDescription>();
            //todo:func
            break;
        }
        case cmd::CmdType::M_ATCK:
        {
            auto mv = commandRes.get_description<cmd::MeleeAttackDescription>();
            //todo:func
            break;
        }
        break;
        case cmd::CmdType::R_ATCK:
        {
            auto mv = commandRes.get_description<cmd::RangeAttackDescription>();
            //todo:func
            break;
        }
        case cmd::CmdType::UNDEF:
        break;
        }
    }

} //namespace sw::mngr
