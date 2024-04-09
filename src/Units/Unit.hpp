#pragma once

#include <cstdint>
#include <limits>
#include <optional>

#include "UnitBuilder.hpp"
#include "UnitTypes.hpp"

namespace sw::units {
class Unit : public IUnit
{
public:
    //ctors
    Unit(UnitClass type, const UnitDescription& ud);
    Unit(UnitClass type, id_type id, hp_type hp, param_type step_count);
    Unit(const Unit &) = default;
    Unit(Unit &&) = default;
    Unit &operator=(const Unit &) = delete;
    Unit &operator=(Unit &&) = delete;
    ~Unit() override = default;
    
    //virt methods
    std::shared_ptr<mngr::cmd::IUnitCommand> process() override;
    void set_unit_position(const map::Point &pos) override;
    map::Point get_unit_position() const override;
    void set_march_position(const map::Point &aim) override;
    std::optional<map::Point> get_march_position() const override;

    [[nodiscard]] std::unique_ptr<IUnit> clone() const override;
    [[nodiscard]] UnitClass get_class() const override;
    [[nodiscard]] id_type get_id() const override;
    [[nodiscard]] UnitState get_state() const override;
    [[nodiscard]] param_type get_step_count() const override;
    hp_type get_hp() const override;
    void change_hp(const hp_type& hp) override;
    void set_main_params(action_type march_action, params_storage_type params, actions_storage_type actions) override;
    param_pair_type::second_type get_param_value(const param_pair_type::first_type& name) const override;


protected:
    const UnitClass _type;
    const id_type _id;
    UnitState _state = UnitState::ALIVE;
    
    map::Point _coord;
    std::optional<map::Point> _march_coord;
    params_storage_type _params;
    action_type _march_method;
    actions_storage_type _priority_actions_storage;
    hp_type _hp;
    param_type _step_count;
};
    
} //namespace sw::units