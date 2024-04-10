#include <gtest/gtest.h>
#include <memory>
#include "../../src/GameManager/GameManager.hpp"
#include "../../src/GameManager/Executors/Loggable/LoggableGameExecutor.hpp"
#include "../../src/Units/LoggableUnit.hpp"
#include "../../src/Units/IUnit.hpp"
#include "../../src/Map/Map.hpp"
#include "../../src/Map/LoggableMap.hpp"
#include "../../src/Units/BuilderTemplates/DefaultUnitsBuilders.hpp"

namespace sw::mngr::test {
using namespace sw::map;
using namespace units;
using namespace units::templates;


TEST(game, spawnTest)
{
    GameManager gm;
    auto map = gm.createMap<LoggableMap<units::IUnit>, LoggableGameExecutor>(10, 10);

    UnitDescription ud0{0, 10, 2};
    UnitDescription ud1{1, 10};
    UnitDescription ud2{2, 25};
    param_type str = 5;
    param_type mRange = 1;
    param_type agility = 3;
    param_type rRange = 4;
    param_type rRange1 = 2;
    param_type cRange = 5;
    param_type mana = 1;
    
    auto unit0 = gm.spawnUnit<LoggableUnit>(ArcherDescription{str, mRange, agility, rRange}, ud0, {5, 0});
    auto unit1 = gm.spawnUnit<LoggableUnit>(WarriorDescription{str, mRange}, ud1, {9, 9});

    //gm.SetMarchForUnit(unit0->getId(), {0, 9});
    gm.setMarchForUnit(unit1->getId(), {0, 0});
    gm.setMarchForUnit(unit0->getId(), {0, 0});

    for(int i = 0; i < 10; ++i)
    {
        gm.waitOneGameTick();
    }

    gm.setMarchForUnit(unit0->getId(), {0, 0});

    for(int i = 0; i < 10; ++i)
    {
        gm.waitOneGameTick();
    }
}
}
