#pragma once
#include "CmdDescriptions.hpp"
namespace sw::mngr::cmd {

class IUnitCommand
{
    public:
    [[nodiscard]] virtual CmdDescription execute() const = 0;
    virtual ~IUnitCommand() = default;
    protected:   
    explicit IUnitCommand(unsigned id) : id_(id) {};
    const unsigned id_;
};

template<class DescrT>
class UnitCommand : public IUnitCommand
{
public:
    using description_type = DescrT;
    UnitCommand(unsigned id, const description_type& descr) : IUnitCommand(id), _descr{descr} {};
    
    CmdDescription execute() const override
    {
        return CmdDescription(IUnitCommand::id_, _descr);
    }
private:
    const description_type _descr;
};
} //namespace sw::mngr::cmd
