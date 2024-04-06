#include "Unit.hpp"
#include <memory>

namespace sw::units {
    
    std::shared_ptr<mngr::cmd::IUnitCommand> Unit::process()
    {
        for (const auto &it: _priority_actions_storage)
        {
            if (auto ptr = it.second(*this, false))
                return ptr;
        }
        return std::shared_ptr<mngr::cmd::IUnitCommand>();
    }
    
    void Unit::set_march(map::Point aim)
    {
        _coord = aim;
    }
    
    std::unique_ptr<IUnit> Unit::clone() const
    {
        return std::make_unique<Unit>(*this);
    }
    
    UnitClass Unit::get_class() const
    {
        return _type;
    }
    
    unsigned Unit::get_id() const
    {
        return _id;
    }
    
    UnitState Unit::get_state() const
    {
        return _state;
    }
}