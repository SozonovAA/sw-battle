#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include "../../src/Map/Map.hpp"
#include "../../src/Units/BuilderTemplates/DefaultUnitsBuilders.hpp"

namespace sw::units::test {
using namespace sw::map;
using namespace units::templates;
// Тест для метода getUnitsAround: получение объектов в радиусе от указанных координат
TEST(unit, meleeAtckTest)
{
    std::shared_ptr<IMap<units::IUnit>> map = std::make_shared<Map<IUnit>>(5, 5);
    hp_type hp = 10;
    param_type str = 5;
    param_type mRange = 1;
    id_type id0 = 0;
    id_type id1 = 0;
    auto unit1 = WarriorBuilder(
        WarriorDescription{hp, str, mRange}, map).create_unit(id0, hp);
    auto unit2 = WarriorBuilder(
        WarriorDescription{hp, str, mRange}, map).create_unit(id1, hp);


    map->addUnit(2, 3, unit1);
    map->addUnit(3, 3, unit2);

    auto com = unit1->process();
    auto cmdDescr = com->execute();
    EXPECT_EQ(cmdDescr._type, CmdType::M_ATCK);
}
}