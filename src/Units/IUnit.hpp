#pragma once

#include <iomanip>
#include <memory>
#include <functional>
#include <ostream>
#include <string>
#include <map>
#include <unordered_map>
#include <optional>

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
    using hp_type = sw::units::hp_type;
    
    virtual std::shared_ptr<mngr::cmd::IUnitCommand> process() = 0;
    virtual void setUnitPosition(const map::Point &pos) = 0;
    virtual map::Point getUnitPosition() const = 0;
    virtual void setMarchPosition(const map::Point &aim) = 0;
    virtual std::optional<map::Point> getMarchPosition() const = 0;
    [[nodiscard]] virtual std::unique_ptr<IUnit> clone() const = 0;
    [[nodiscard]] virtual UnitClass getClass() const = 0;
    [[nodiscard]] virtual id_type getId() const = 0;
    [[nodiscard]] virtual UnitState getState() const = 0;
    [[nodiscard]] virtual param_type getStepCount() const = 0;
    virtual hp_type getHp() const = 0;
    virtual void changeHp(const hp_type& hp) = 0;
    virtual void
    setMainParams(action_type march_action, params_storage_type params, actions_storage_type actions) = 0;
    virtual param_pair_type::second_type getParamValue(const param_pair_type::first_type& name) const = 0;
    
    virtual ~IUnit() = default;
    
    friend std::ostream &operator<<(std::ostream &os, const IUnit &unit)
    {
        switch (unit.getClass())
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
            case UnitClass::MAG:
                os << "M";
                break;
        }
        os << "(" << std::setw(2) << unit.getId() << "," << std::setw(2) << unit.getHp() << ")" ;
        return os;
    };
};
    
}