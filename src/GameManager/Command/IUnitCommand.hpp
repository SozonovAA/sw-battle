#pragma once
#include "CmdDescriptions.hpp"
namespace sw::mngr::cmd {

class IUnitCommand
{
    public:
    virtual CmdDescription execute() const = 0;
    virtual ~IUnitCommand() = default;
    protected:   
    IUnitCommand(unsigned id) : id_(id) {};
    const unsigned id_;
};

template<class DescrT>
class UnitCommand : public IUnitCommand
{
public:
    using description_type = DescrT;
    UnitCommand(unsigned id, const description_type& descr) : IUnitCommand(id), descr_{descr} {};

    virtual CmdDescription execute() const override
    {
        return CmdDescription(IUnitCommand::id_, descr_);
    }
private:
    const description_type descr_;
};
} //namespace sw::mngr::cmd
