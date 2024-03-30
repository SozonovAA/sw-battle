#pragma once
#include <memory>

#include "Unit.hpp"

namespace sw::units {

//todo: concept
template<UnitClass Type, class UnitT = Unit>
class UnitBuilder
{
    public:
    using unit_type = UnitT;
    std::unique_ptr<IUnit> create_unit(unsigned id)
    {
        return std::make_unique<unit_type>(unit_type(Type, id));
    };

};


} //namespace sw::units