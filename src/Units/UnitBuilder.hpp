#pragma once
#include <memory>

#include "IUnit.hpp"

namespace sw::units {

//todo: concept
template<UnitClass Type, class UnitT>
class UnitBuilder
{
    public:
    using unit_type = UnitT;

    using param_type = IUnit::param_type;
    using action_type = IUnit::action_type;
    using params_storage_type = IUnit::params_storage_type;
    using actions_storage_type = IUnit::actions_storage_type;

    std::unique_ptr<IUnit> create_unit(unsigned id)
    {
        auto ret = std::make_unique<unit_type>(unit_type(Type, id));
        ret->_priority_actions_storage = _actions;
        ret->_params = _params;
        return ret;
    };

    bool add_param_with_default_value(const param_type& par)
    {
        return _params.try_emplace(par).second;
    };

    bool add_action_by_priority(actions_storage_type::size_type prior, action_type act)
    {
        
        return _actions.try_emplace(prior, act).second;
    };

    private:
    params_storage_type _params;
    actions_storage_type _actions;
};


} //namespace sw::units