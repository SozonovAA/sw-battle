#pragma once
#include "Command/IUnitCommand.hpp"
namespace sw::mngr
{
class IGameSystem
{
    public:
    void execute(const cmd::IUnitCommand& cmd);
    virtual ~IGameSystem() = default;
};

}//namespace sw::mngr