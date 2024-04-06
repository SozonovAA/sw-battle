#pragma once

#include <memory>
#include <utility>

#include "IUnit.hpp"

namespace sw::units {

static std::shared_ptr<mngr::cmd::IUnitCommand> DefaultMarchMethod(const units::IUnit &uRef)
{
    using namespace mngr::cmd;
    map::Point res;
    const unsigned stepCount = 1;
    const auto currPos = uRef.get_unit_position();
    const auto marchPos = uRef.get_march_position();
    const auto deltaPos = marchPos - currPos;

    if (std::abs(deltaPos._x) >= stepCount) {
        res._x = stepCount * (std::signbit(deltaPos._x) ? -1 : 1);
    }
    else {
        res._x = deltaPos._x;
    }
    if (std::abs(deltaPos._y) >= stepCount) {
        res._y = stepCount * (std::signbit(deltaPos._y) ? -1 : 1);
    }
    else {
        res._y = deltaPos._y;
    }
    return std::make_shared<UnitCommand<MoveDescription>>(uRef.get_id(), MoveDescription{res._x, res._y});
}
//todo: concept
template<UnitClass Type, class UnitT>
class UnitBuilder
{
public:
    static_assert(std::is_base_of<IUnit, UnitT>::value, "UnitT must be derived from IUnit!");
    
    using unit_type = UnitT;
    using id_type = IUnit::id_type;
    using hp_type = IUnit::hp_type;
    using param_pair_type = IUnit::param_pair_type;
    using action_type = IUnit::action_type;
    using params_storage_type = IUnit::params_storage_type;
    using actions_storage_type = IUnit::actions_storage_type;
    
    std::unique_ptr<IUnit> create_unit(const id_type& id, const hp_type& hp)
    {
        auto ret = std::make_unique<unit_type>(unit_type(Type, id, hp));
        ret->set_main_params(_march_method, _params, _actions);
        return ret;
    };
    
    bool add_param(const param_pair_type::first_type &name, const param_pair_type::second_type &value)
    {
        return _params.try_emplace(name, value).second;
    };
    
    void set_march_method(action_type act)
    {
        _march_method = std::move(act);
    };
    
    bool add_action_by_priority(actions_storage_type::size_type prior, action_type act)
    {
        return _actions.try_emplace(prior, act).second;
    };

private:
    params_storage_type _params;
    actions_storage_type _actions;
    action_type _march_method;
};
} //namespace sw::units