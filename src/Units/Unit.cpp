#include "Unit.hpp"
#include <memory>
namespace sw::units {

bool Unit::march_process()
{
    bool res;

    for(const auto& it : _priority_actions_storage)
    {
        if(it.second(*this, false))
            break;
    }

    //todo: compare coordinates with aims
    return res;
}

void Unit::set_march(unsigned x, unsigned y)
{

}

std::unique_ptr<IUnit> Unit::clone() const
{
    return std::make_unique<Unit>(*this);
}

UnitClass Unit::get_class() const
{
    return _type;
}

unsigned Unit::get_id() const
{
    return _id;
}

UnitState Unit::get_state() const
{
    return _state;
}
}