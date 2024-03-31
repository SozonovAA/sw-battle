#pragma once
#include <cstdint>

#include "UnitBuilder.hpp"

namespace sw::units {
class Unit : public IUnit
{
    public:
    //ctors
    Unit(UnitClass type, unsigned id) : _type(type), _id(id) {};
    Unit(const Unit&) = default;
    Unit(Unit&&) = default;

    Unit& operator=(const Unit&) = delete;
    Unit& operator=(Unit&&) = delete;
    virtual ~Unit() = default;

    //virt methods
    virtual std::shared_ptr<mngr::cmd::IUnitCommand> process() override;
    virtual void set_march(map::Point aim) override;
    virtual std::unique_ptr<IUnit> clone() const override;
    virtual UnitClass get_class() const override;
    virtual unsigned get_id() const override;
    virtual UnitState get_state() const override;

    template<UnitClass U, class T> friend class UnitBuilder;

    protected:
    const UnitClass _type;
    const unsigned _id;
    UnitState _state;

    map::Point _coord;
    params_storage_type _params;
    //todo: special march method
    actions_storage_type _priority_actions_storage;
};

} //namespace sw::units