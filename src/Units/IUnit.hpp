#pragma once

#include <iomanip>
#include <memory>
#include <functional>
#include <ostream>
#include <string>
#include <map>
#include <unordered_map>

#include "UnitTypes.hpp"
#include "../Map/Coords.hpp"
#include "../GameManager/Command/IUnitCommand.hpp"

namespace sw::units {
enum class UnitState
{
    UNDEF = 0,
    ALIVE,
    DEAD
};

class IUnit
{
public:
    using param_pair_type = std::pair<std::string, sw::mngr::cmd::param_type>;
    using action_type = std::function<std::shared_ptr<sw::mngr::cmd::IUnitCommand>(std::shared_ptr<IUnit>)>;
    using params_storage_type = std::unordered_map<std::string, sw::mngr::cmd::param_type>;
    using actions_storage_type = std::map<unsigned, action_type, std::less<unsigned>>;
    using id_type = sw::mngr::cmd::id_type;
    using hp_type = hp_type;
    
    virtual std::shared_ptr<mngr::cmd::IUnitCommand> process() = 0;
    virtual void set_unit_position(const map::Point &pos) = 0;
    virtual map::Point get_unit_position() const = 0;
    virtual void set_march_position(const map::Point &aim) = 0;
    virtual map::Point get_march_position() const = 0;
    [[nodiscard]] virtual std::unique_ptr<IUnit> clone() const = 0;
    [[nodiscard]] virtual UnitClass get_class() const = 0;
    [[nodiscard]] virtual id_type get_id() const = 0;
    [[nodiscard]] virtual UnitState get_state() const = 0;
    [[nodiscard]] virtual param_type get_step_count() const = 0;
    virtual hp_type get_hp() const = 0;
    virtual void change_hp(const hp_type& hp) = 0;
    virtual void
    set_main_params(action_type march_action, params_storage_type params, actions_storage_type actions) = 0;
    virtual param_pair_type::second_type get_param_value(const param_pair_type::first_type& name) const = 0;
    
    virtual ~IUnit() = default;
    
    // Перегрузка оператора вывода для вывода карты в поток
    friend std::ostream &operator<<(std::ostream &os, const IUnit &unit)
    {
        switch (unit.get_class())
        {
            case UnitClass::UNDEF:
                os << "U";
                break;
            case UnitClass::WAR:
                os << "W";
                break;
            case UnitClass::ARCH:
                os << "A";
                break;
        }
        os << "(" << std::setw(2) << unit.get_id() << "," << std::setw(2) << unit.get_hp() << ")" ;
        return os;
    };
};
    
}