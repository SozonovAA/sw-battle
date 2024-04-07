#pragma once

#include "Command/IUnitCommand.hpp"

namespace sw::mngr {
class IGameSystem
{
public:
    virtual bool execute(const cmd::IUnitCommand &cmd) = 0;
    virtual ~IGameSystem() = default;
};
    
}//namespace sw::mngr