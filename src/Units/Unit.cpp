#include "Unit.hpp"
#include <memory>

namespace sw::units {
    
    Unit::Unit(UnitClass type, unsigned int id) : _type(type), _id(id) {}
    
    std::shared_ptr<mngr::cmd::IUnitCommand> Unit::process()
    {
        for (const auto &it: _priority_actions_storage)
        {
            if (auto ptr = it.second(*this, false))
                return ptr;
        }
        return {};
    }
    
    void Unit::set_unit_position(const map::Point& aim)
    {
        _coord = aim;
    }
    map::Point Unit::get_unit_position()
    {
        return _coord;
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
    
    void Unit::set_main_params(IUnit::action_type march_action, IUnit::params_storage_type params,
                               IUnit::actions_storage_type actions)
    {
        _priority_actions_storage = actions;
        _params = params;
        _march_method = march_action;
    }
}