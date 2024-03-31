#pragma once
#include <cstdint>

#include "UnitBuilder.hpp"

namespace sw::units {
class Unit : public IUnit
{
    public:
    //ctors
    Unit(UnitClass type, unsigned id) : IUnit(type, id) {};
    Unit(const Unit&) = default;
    Unit(Unit&&) = default;

    Unit& operator=(const Unit&) = delete;
    Unit& operator=(Unit&&) = delete;
    virtual ~Unit() = default;

    //virt methods
    virtual bool march_process() override;
    virtual void set_march(unsigned x, unsigned y) override;
    virtual std::unique_ptr<IUnit> clone() const override;

    template<UnitClass U, class T> friend class UnitBuilder;

    protected:
    params_storage_type _params;
    //todo: special march method
    actions_storage_type _priority_actions_storage;
};

} //namespace sw::units