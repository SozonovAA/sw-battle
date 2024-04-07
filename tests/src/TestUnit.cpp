#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include "../../src/Map/Map.hpp"
#include "../../src/Units/BuilderTemplates/DefaultUnitsBuilders.hpp"

namespace sw::units::test {
using namespace sw::map;
using namespace units::templates;
TEST(unit, skipTest)
{
    std::shared_ptr<IMap<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);

    UnitDescription ud0{0, 10};
    param_type str = 5;
    param_type mRange = 1;
    auto unit0 = WarriorBuilder(
        WarriorDescription{str, mRange}, map).create_unit(ud0);

    Point _0 = {2, 3};
    map->addUnit(_0._x, _0._y, unit0);
    unit0->set_unit_position(_0);
    unit0->set_march_position(_0);

    auto com = unit0->process();
    auto cmdDescr = com->execute();
    EXPECT_EQ(cmdDescr._type, CmdType::SKIP);
    SkipDescription expect{};
    EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::SKIP>());
}


TEST(unit, marchTest)
{
    std::shared_ptr<IMap<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);

    UnitDescription ud0{0, 10};
    param_type str = 5;
    param_type mRange = 1;
    auto unit0 = WarriorBuilder(
        WarriorDescription{str, mRange}, map).create_unit(ud0);

    Point _0 = {2, 3};
    Point _1 = {10, 10};
    map->addUnit(_0._x, _0._y, unit0);
    unit0->set_unit_position(_0);
    unit0->set_march_position(_1);

    auto com = unit0->process();
    auto cmdDescr = com->execute();
    EXPECT_EQ(cmdDescr._type, CmdType::MOVE);
    MoveDescription expect{1, 1};
    EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::MOVE>());
    EXPECT_EQ(cmdDescr.get_description<CmdType::MOVE>(), expect);
}


TEST(unit, meleeAtckTest)
{
    std::shared_ptr<IMap<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);

    UnitDescription ud0{0, 10};
    UnitDescription ud1{1, 10};
    param_type str = 5;
    param_type mRange = 1;
    auto unit0 = WarriorBuilder(
        WarriorDescription{str, mRange}, map).create_unit(ud0);
    auto unit1 = WarriorBuilder(
        WarriorDescription{str, mRange}, map).create_unit(ud1);

    Point _0 = {2, 3};
    map->addUnit(_0._x, _0._y, unit0);
    unit0->set_unit_position(_0);

    _0 = {3, 3};
    map->addUnit(_0._x, _0._y, unit1);
    unit1->set_unit_position(_0);

    //simple melee atack test
    auto com = unit0->process();
    auto cmdDescr = com->execute();
    EXPECT_EQ(cmdDescr._type, CmdType::M_ATCK);
    MeleeAttackDescription expect{1, 5};
    EXPECT_EQ(cmdDescr.get_description<CmdType::M_ATCK>(), expect);

    //lower HP unit simple melee atack test
    UnitDescription ud2{2, 9};
    auto unit2 = WarriorBuilder(
        WarriorDescription{str, mRange}, map).create_unit(ud2);

    _0 = {2, 2};
    map->addUnit(_0._x, _0._y, unit2);
    unit2->set_unit_position(_0);

    com = unit0->process();
    auto cmdDescr1 = com->execute();
    EXPECT_EQ(cmdDescr1._type, CmdType::M_ATCK);
    MeleeAttackDescription expect1{2, 5};
    EXPECT_EQ(cmdDescr.get_description<CmdType::M_ATCK>(), expect);


    //lower HP + lower ID unit simple melee atack test
    UnitDescription ud3{1, 9};
    auto unit3 = WarriorBuilder(
        WarriorDescription{str, mRange}, map).create_unit(ud2);

    _0 = {1, 3};
    map->addUnit(_0._x, _0._y, unit2);
    unit2->set_unit_position(_0);

    com = unit0->process();
    auto cmdDescr2 = com->execute();
    EXPECT_EQ(cmdDescr1._type, CmdType::M_ATCK);
    MeleeAttackDescription expect2{1, 5};
    EXPECT_EQ(cmdDescr.get_description<CmdType::M_ATCK>(), expect);

    com = unit2->process();
    auto cmdDescr3 = com->execute();
    EXPECT_EQ(cmdDescr1._type, CmdType::M_ATCK);
    MeleeAttackDescription expect3{0, 5};
    EXPECT_EQ(cmdDescr.get_description<CmdType::M_ATCK>(), expect);
}



TEST(unit, marchAndAttackTest)
{
    std::shared_ptr<IMap<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);

    UnitDescription ud0{0, 10};
    UnitDescription ud1{1, 10};
    param_type str = 5;
    param_type mRange = 1;
    auto unit0 = WarriorBuilder(
        WarriorDescription{str, mRange}, map).create_unit(ud0);
    auto unit1 = WarriorBuilder(
        WarriorDescription{str, mRange}, map).create_unit(ud1);


    Point _0 = {2, 3};
    Point _1 = {4, 5};
    map->addUnit(_0._x, _0._y, unit0);
    map->addUnit(_1._x, _1._y, unit1);

    unit0->set_unit_position(_0);
    unit1->set_unit_position(_1);
    unit0->set_march_position({4, 5});

    //move
    auto com = unit0->process();
    auto cmdDescr = com->execute();
    EXPECT_EQ(cmdDescr._type, CmdType::MOVE);
    MoveDescription expect{1, 1};
    EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::MOVE>());
    EXPECT_EQ(cmdDescr.get_description<CmdType::MOVE>(), expect);

    auto uPos = unit0->get_unit_position();
    auto newUPos = uPos + Point{expect.delta_x, expect.delta_y};
    map->moveUnit(uPos._x, uPos._y, newUPos._x, newUPos._y);
    unit0->set_unit_position( newUPos );

    //mAtck
    com = unit0->process();
    auto cmdDescr1 = com->execute();
    EXPECT_EQ(cmdDescr1._type, CmdType::M_ATCK);
    MeleeAttackDescription expect1{1, 5};
    EXPECT_NO_THROW(auto d = cmdDescr1.get_description<CmdType::M_ATCK>());
    EXPECT_EQ(cmdDescr1.get_description<CmdType::M_ATCK>(), expect1);

    //mAtck + kill
    com = unit0->process();
    auto cmdDescr2 = com->execute();
    EXPECT_EQ(cmdDescr2._type, CmdType::M_ATCK);
    MeleeAttackDescription expect2{1, 5};
    EXPECT_NO_THROW(auto d = cmdDescr2.get_description<CmdType::M_ATCK>());
    EXPECT_EQ(cmdDescr2.get_description<CmdType::M_ATCK>(), expect2);

    auto pos = unit1->get_unit_position();
    auto deletedUnit = map->deleteUnit(pos._x, pos._y);
    EXPECT_EQ(deletedUnit, unit1);

    //move after kill
    com = unit0->process();
    auto cmdDescr3 = com->execute();
    EXPECT_EQ(cmdDescr3._type, CmdType::MOVE);
    MoveDescription expect3{1, 1};
    EXPECT_NO_THROW(auto d = cmdDescr3.get_description<CmdType::MOVE>());
    EXPECT_EQ(cmdDescr3.get_description<CmdType::MOVE>(), expect3);
    
    auto uPos1 = unit0->get_unit_position();
    auto newUPos1 = uPos1 + Point{expect.delta_x, expect.delta_y};
    map->moveUnit(uPos1._x, uPos1._y, newUPos1._x, newUPos1._y);
    unit0->set_unit_position( newUPos1 );

    //skip
    com = unit0->process();
    auto cmdDescr4 = com->execute();
    EXPECT_EQ(cmdDescr4._type, CmdType::SKIP);
    EXPECT_NO_THROW(auto d = cmdDescr4.get_description<CmdType::SKIP>());
}
}