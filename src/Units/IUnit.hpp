#pragma once
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
    using param_type = std::pair<std::string, int>;
    using action_type = std::function<std::shared_ptr<sw::mngr::cmd::IUnitCommand>(IUnit&, bool)>;
    using params_storage_type = std::unordered_map<std::string, int>;
    using actions_storage_type = std::map<unsigned, action_type>;

    //todo: get param value method

    virtual std::shared_ptr<mngr::cmd::IUnitCommand> process() = 0;
    virtual void set_march(map::Point aim) = 0;
    virtual std::unique_ptr<IUnit> clone() const = 0;
    virtual UnitClass get_class() const = 0;
    virtual unsigned get_id() const = 0;
    virtual UnitState get_state() const = 0;
    virtual ~IUnit() = default;

    // Перегрузка оператора вывода для вывода карты в поток
    friend std::ostream& operator<<(std::ostream& os, const IUnit& map){
        switch (map.get_class()) {
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
        return os;
    };
};

}