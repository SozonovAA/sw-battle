#pragma once

#include <cstdint>

#include "UnitBuilder.hpp"

namespace sw::units {
    class Unit : public IUnit
    {
    public:
        //ctors
        Unit(UnitClass type, unsigned id);
        Unit(const Unit &) = default;
        Unit(Unit &&) = default;
        Unit &operator=(const Unit &) = delete;
        Unit &operator=(Unit &&) = delete;
        ~Unit() override = default;
        
        //virt methods
        std::shared_ptr<mngr::cmd::IUnitCommand> process() override;
        void set_unit_position(const map::Point& aim) override;
        map::Point get_unit_position() override;
        [[nodiscard]] std::unique_ptr<IUnit> clone() const override;
        [[nodiscard]] UnitClass get_class() const override;
        [[nodiscard]] unsigned get_id() const override;
        [[nodiscard]] UnitState get_state() const override;
        void set_main_params(action_type march_action, params_storage_type params, actions_storage_type actions) override;
    
    protected:
        const UnitClass _type;
        const unsigned _id;
        UnitState _state;
        
        map::Point _coord;
        params_storage_type _params;
        action_type _march_method;
        actions_storage_type _priority_actions_storage;
    };
    
} //namespace sw::units