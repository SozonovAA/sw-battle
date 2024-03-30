#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

#include "IUnit.hpp"
#include "UnitTypes.hpp"
namespace sw::units {
class Unit : public IUnit
{
    public:
    using param_type = std::pair<std::string, int>;
    using params_storage_type = std::unordered_map<std::string, int>;
    using action_type = std::function<bool(Unit&, bool)>;
    using actions_storage_type = std::vector<action_type>;

    //ctors
    Unit(UnitClass type, unsigned id) : _type(type), _id(id) {};
    Unit(const Unit&) = default;
    Unit(Unit&&) = default;

    Unit& operator=(const Unit&) = delete;
    Unit& operator=(Unit&&) = delete;
    virtual ~Unit() = default;

    //virt methods
    virtual void update() override;
    virtual std::unique_ptr<IUnit> clone() const override;

    protected:
    const UnitClass _type;
    const unsigned _id;
    unsigned _hp = 0;
    unsigned _x = 0, _y = 0;
    params_storage_type _params;
    actions_storage_type _priority_actions_storage;
};

} //namespace sw::units