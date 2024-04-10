#include "Unit.hpp"
#include <memory>

namespace sw::units {

Unit::Unit(UnitClass type, id_type id, hp_type hp, param_type step_count) : _type(type), _id(id), _hp(hp), _step_count(step_count) {}

Unit::Unit(UnitClass type, const UnitDescription& ud): 
    _type(type),
    _id(ud.id),
    _hp(ud.hp),
    _step_count(ud.step_count) {}


std::shared_ptr<mngr::cmd::IUnitCommand> Unit::process()
{
    for (const auto &it: _priority_actions_storage)
    {
        if (auto ptr = it.second(std::make_shared<Unit>(*this)))
            return ptr;
    }
    if(_coord == _march_coord)
        _march_coord.reset();

    if (_march_coord.has_value() && (_coord != _march_coord) && _march_method) {
        return _march_method(std::make_shared<Unit>(*this));
    }
    return std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::SkipDescription>>(_id, mngr::cmd::SkipDescription{});
}

void Unit::setUnitPosition(const map::Point &aim)
{
    _coord = aim;
}
map::Point Unit::getUnitPosition() const
{
    return _coord;
}

void Unit::setMarchPosition(const map::Point &aim)
{
    _march_coord = aim;
}
std::optional<map::Point> Unit::getMarchPosition() const
{
    return _march_coord;
}

std::unique_ptr<IUnit> Unit::clone() const
{
    return std::make_unique<Unit>(*this);
}

UnitClass Unit::getClass() const
{
    return _type;
}

IUnit::id_type Unit::getId() const
{
    return _id;
}

UnitState Unit::getState() const
{
    return _state;
}

param_type Unit::getStepCount() const 
{
    return _step_count;
}

Unit::hp_type Unit::getHp() const
{
    return _hp;
}

void Unit::changeHp(const Unit::hp_type& hp)
{
    _hp += hp;
    if(_hp <= 0)
        _state = UnitState::DEAD;
}

void Unit::setMainParams(IUnit::action_type march_action, IUnit::params_storage_type params,
                           IUnit::actions_storage_type actions)
{
    _priority_actions_storage = std::move(actions);
    _params = std::move(params);
    _march_method = march_action;
}

IUnit::param_pair_type::second_type Unit::getParamValue(const param_pair_type::first_type& name) const
{
    if(const auto it = _params.find(name); it == _params.end())
        throw std::runtime_error("Unknown param: " + name + " !");
    else
        return it->second;
}

} //namespace sw::units