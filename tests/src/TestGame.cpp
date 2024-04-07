#include <gtest/gtest.h>
#include <memory>
#include "../../src/GameManager/GameManager.hpp"
#include "../../src/GameManager/GameSystem.hpp"
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
    auto gameSystem = gm.CreateGameSystem<GameSystem>();

    UnitDescription ud0{0, 10};
    UnitDescription ud1{1, 10};
    param_type str = 5;
    param_type mRange = 1;
    param_type agility = 3;
    param_type rRange = 4;
    param_type rRange1 = 2;

    const auto archDescr0 = ArcherDescription{str, mRange, agility, rRange};
    const auto archDescr1 = ArcherDescription{str, mRange, agility, rRange1};

    Point _0 = {0, 0};
    Point _1 = {9, 9};

    auto unit0 = gm.SpawnUnit(archDescr0, ud0, _0);
    auto unit1 = gm.SpawnUnit(archDescr1, ud1, _1);

    //gm.SetMarchForUnit(unit0->get_id(), {0, 9});
    gm.SetMarchForUnit(unit1->get_id(), {0, 0});

    for(int i = 0; i < 10; ++i)
    {
        gm.WaitOneGameTick();
        std::cout << *map;
    }

}
}
