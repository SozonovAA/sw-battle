#pragma once

#include <memory>
#include <utility>

#include "IUnit.hpp"
#include "UnitTypes.hpp"

namespace sw::units {
//todo: concept
template<UnitClass Type, class UnitT>
class UnitBuilder
{
public:
    static_assert(std::is_base_of<IUnit, UnitT>::value, "UnitT must be derived from IUnit!");
    const static UnitClass unit_class = Type;
    using unit_type = UnitT;
    using id_type = IUnit::id_type;
    using hp_type = IUnit::hp_type;
    using param_pair_type = IUnit::param_pair_type;
    using action_type = IUnit::action_type;
    using params_storage_type = IUnit::params_storage_type;
    using actions_storage_type = IUnit::actions_storage_type;
    
    std::shared_ptr<IUnit> createUnit(const UnitDescription& ud)
    {
        auto ret = std::make_unique<unit_type>(unit_type(Type, ud));
        ret->setMainParams(_march_method, _params, _actions);
        return ret;
    };
    
    bool addParam(const param_pair_type::first_type &name, const param_pair_type::second_type &value)
    {
        return _params.try_emplace(name, value).second;
    };
    
    void setMarchMethod(action_type act)
    {
        _march_method = std::move(act);
    };
    
    bool addActionByPriority(actions_storage_type::size_type prior, action_type act)
    {
        return _actions.try_emplace(prior, act).second;
    };

private:
    params_storage_type _params;
    actions_storage_type _actions;
    action_type _march_method;
};

} //namespace sw::units