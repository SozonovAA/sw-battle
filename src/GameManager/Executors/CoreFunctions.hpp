#pragma once
#include "../../Map/IMap.hpp"
#include "../../Units/IUnit.hpp"

namespace sw::mngr {
using namespace units;
using namespace cmd;
using namespace map;
static bool UpdateUnitPosition(std::shared_ptr<IUnit> unit, MoveDescription expect, std::shared_ptr<IMap<IUnit>> map)
{
    bool res = false;
    auto uPos1 = unit->get_unit_position();
    auto newUPos1 = uPos1 + Point{expect.delta_x, expect.delta_y};
    if(!map->moveUnit(uPos1._x, uPos1._y, newUPos1._x, newUPos1._y))
    {
        unit->set_unit_position( newUPos1 );
        res = true;
    }
    return res;
}
}