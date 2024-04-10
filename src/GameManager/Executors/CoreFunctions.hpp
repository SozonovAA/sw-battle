#pragma once
#include "../../Map/IMap.hpp"
#include "../../Units/IUnit.hpp"

namespace sw::mngr {
using namespace units;
using namespace cmd;
using namespace map;
static bool updateUnitPosition(std::shared_ptr<IUnit> unit, MoveDescription expect, std::shared_ptr<IMap<IUnit>> map)
{
    bool res = false;
    const auto uPos1 = unit->getUnitPosition();
    const auto newUPos1 = uPos1 + Point{expect.deltax, expect.deltay};
    if(!map->moveUnit(uPos1.x, uPos1.y, newUPos1.x, newUPos1.y))
    {
        unit->setUnitPosition( newUPos1 );
        res = true;
    }
    return res;
}
}