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

}