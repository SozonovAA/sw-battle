#pragma once

#include "Command/IUnitCommand.hpp"

namespace sw::mngr {
class IGameExecutor
{
public:
    virtual bool execute(const cmd::IUnitCommand &cmd) = 0;
    virtual ~IGameExecutor() = default;
};
    
}//namespace sw::mngr