#include "Unit.hpp"
#include <memory>
namespace sw::units {

void Unit::update()
{

}


std::unique_ptr<IUnit> Unit::clone() const
{
    return std::make_unique<Unit>(*this);

}

}