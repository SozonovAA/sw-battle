#include <gtest/gtest.h>
#include <memory>
#include "../../src/GameManager/GameManager.hpp"
#include "../../src/GameManager/GameSystem.hpp"
#include "../../src/GameManager/LoggableGameSystem.hpp"
#include "../../src/Units/LoggableUnit.hpp"
#include "../../src/Units/IUnit.hpp"
#include "../../src/Map/Map.hpp"
#include "../../src/Units/BuilderTemplates/DefaultUnitsBuilders.hpp"

namespace sw::mngr::test {
using namespace sw::map;
using namespace units;
using namespace units::templates;


TEST(game, spawnTest)
{
    std::shared_ptr<Map<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);
    GameManager gm(map);
    auto gameSystem = gm.CreateGameSystem<LoggableGameSystem>();

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
    
    auto unit0 = gm.SpawnUnit<LoggableUnit>(ArcherDescription{str, mRange, agility, rRange}, ud0, {5, 0});
    auto unit1 = gm.SpawnUnit<LoggableUnit>(WarriorDescription{str, mRange}, ud1, {9, 9});

    //gm.SetMarchForUnit(unit0->get_id(), {0, 9});
    gm.SetMarchForUnit(unit1->get_id(), {0, 0});
    gm.SetMarchForUnit(unit0->get_id(), {0, 0});

    for(int i = 0; i < 10; ++i)
    {
        gm.WaitOneGameTick();
        std::cout << *map;
    }


    //gm.SetMarchForUnit(unit0->get_id(), {0, 9});
    gm.SetMarchForUnit(unit0->get_id(), {0, 0});

    for(int i = 0; i < 10; ++i)
    {
        gm.WaitOneGameTick();
        std::cout << *map;
    }
}
}
