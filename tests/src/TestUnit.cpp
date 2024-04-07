#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include "../../src/Map/Map.hpp"
#include "../../src/Units/BuilderTemplates/DefaultUnitsBuilders.hpp"

namespace sw::units::test {
using namespace sw::map;
using namespace units::templates;

void UpdateUnitPosition(std::shared_ptr<IUnit> unit0, MoveDescription expect, std::shared_ptr<IMap<IUnit>> map)
{
    auto uPos1 = unit0->get_unit_position();
    auto newUPos1 = uPos1 + Point{expect.delta_x, expect.delta_y};
    map->moveUnit(uPos1._x, uPos1._y, newUPos1._x, newUPos1._y);
    unit0->set_unit_position( newUPos1 );
}

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

    UpdateUnitPosition(unit0, cmdDescr.get_description<CmdType::MOVE>(), map);

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
    

    UpdateUnitPosition(unit0, cmdDescr3.get_description<CmdType::MOVE>(), map);

    //skip
    com = unit0->process();
    auto cmdDescr4 = com->execute();
    EXPECT_EQ(cmdDescr4._type, CmdType::SKIP);
    EXPECT_NO_THROW(auto d = cmdDescr4.get_description<CmdType::SKIP>());
}


TEST(unit, archerFullTest)
{

    UnitDescription ud0{0, 10};
    UnitDescription ud1{1, 10};
    param_type str = 5;
    param_type mRange = 1;
    param_type agility = 3;
    param_type rRange = 4;
    param_type rRange1 = 2;

    {
        std::shared_ptr<IMap<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);

        auto unit0 = ArcherBuilder(
            ArcherDescription{str, mRange, agility, rRange}, map).create_unit(ud0);
        auto unit1 = ArcherBuilder(
            ArcherDescription{str, mRange, agility, rRange1}, map).create_unit(ud1);

        Point _0 = {0, 0};
        Point _1 = {1, 1};
        map->addUnit(_0._x, _0._y, unit0);
        map->addUnit(_1._x, _1._y, unit1);

        unit0->set_unit_position(_0);
        unit1->set_unit_position(_1);

        auto com = unit0->process();
        auto cmdDescr = com->execute();
        EXPECT_EQ(cmdDescr._type, CmdType::M_ATCK);
        MeleeAttackDescription expect{1, static_cast<hp_type>(str)};
        EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::M_ATCK>());
        EXPECT_EQ(cmdDescr.get_description<CmdType::M_ATCK>(), expect);

    }

    {
        std::shared_ptr<IMap<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);

        auto unit0 = ArcherBuilder(
            ArcherDescription{str, mRange, agility, rRange}, map).create_unit(ud0);
        auto unit1 = ArcherBuilder(
            ArcherDescription{str, mRange, agility, rRange}, map).create_unit(ud1);

        Point _0 = {0, 0};
        Point _1 = {2, 2};
        map->addUnit(_0._x, _0._y, unit0);
        map->addUnit(_1._x, _1._y, unit1);

        unit0->set_unit_position(_0);
        unit1->set_unit_position(_1);

        auto com = unit0->process();
        auto cmdDescr = com->execute();
        EXPECT_EQ(cmdDescr._type, CmdType::R_ATCK);
        RangeAttackDescription expect{1, rRange ,static_cast<hp_type>(agility)};
        EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::R_ATCK>());
        EXPECT_EQ(cmdDescr.get_description<CmdType::R_ATCK>(), expect);

    }

    {
        std::shared_ptr<IMap<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);

        auto unit0 = ArcherBuilder(
            ArcherDescription{str, mRange, agility, rRange}, map).create_unit(ud0);
        auto unit1 = ArcherBuilder(
            ArcherDescription{str, mRange, agility, rRange1}, map).create_unit(ud1);

        Point _0 = {0, 0};
        Point _1 = {4, 4};
        map->addUnit(_0._x, _0._y, unit0);
        map->addUnit(_1._x, _1._y, unit1);

        unit0->set_unit_position(_0);
        unit1->set_unit_position(_1);

        auto com = unit0->process();
        auto cmdDescr = com->execute();
        EXPECT_EQ(cmdDescr._type, CmdType::R_ATCK);
        RangeAttackDescription expect{1, rRange, static_cast<hp_type>(agility)};
        EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::R_ATCK>());
        EXPECT_EQ(cmdDescr.get_description<CmdType::R_ATCK>(), expect);

    }

    {
        std::shared_ptr<Map<units::IUnit>> map = std::make_shared<Map<IUnit>>(10, 10);

        auto unit0 = ArcherBuilder(
            ArcherDescription{str, mRange, agility, rRange}, map).create_unit(ud0);
        auto unit1 = ArcherBuilder(
            ArcherDescription{str, mRange, agility, rRange1}, map).create_unit(ud1);

        Point _0 = {0, 0};
        Point _1 = {5, 5};
        map->addUnit(_0._x, _0._y, unit0);
        map->addUnit(_1._x, _1._y, unit1);

        unit0->set_unit_position(_0);
        unit1->set_unit_position(_1);
        
        unit0->set_march_position(_1);

        auto com = unit0->process();
        {
            auto cmdDescr = com->execute();
            EXPECT_EQ(cmdDescr._type, CmdType::MOVE);
            MoveDescription expect{1, 1};
            EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::MOVE>());
            EXPECT_EQ(cmdDescr.get_description<CmdType::MOVE>(), expect);

            UpdateUnitPosition(unit0, cmdDescr.get_description<CmdType::MOVE>(), map);
        }
        
        com = unit0->process();
        {
            auto cmdDescr = com->execute();
            EXPECT_EQ(cmdDescr._type, CmdType::R_ATCK);
            RangeAttackDescription expect{1, rRange, static_cast<hp_type>(agility)};
            EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::R_ATCK>());
            EXPECT_EQ(cmdDescr.get_description<CmdType::R_ATCK>(), expect);
        }

        unit1->set_march_position(_0);
        com = unit1->process();
        {
            auto cmdDescr = com->execute();
            EXPECT_EQ(cmdDescr._type, CmdType::MOVE);
            MoveDescription expect{-1, -1};
            EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::MOVE>());
            EXPECT_EQ(cmdDescr.get_description<CmdType::MOVE>(), expect);

            UpdateUnitPosition(unit1, cmdDescr.get_description<CmdType::MOVE>(), map);
        }

        com = unit0->process();
        {
            auto cmdDescr = com->execute();
            EXPECT_EQ(cmdDescr._type, CmdType::R_ATCK);
            RangeAttackDescription expect{1, rRange, static_cast<hp_type>(agility)};
            EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::R_ATCK>());
            EXPECT_EQ(cmdDescr.get_description<CmdType::R_ATCK>(), expect);
        }        
        com = unit1->process();
        {
            auto cmdDescr = com->execute();
            EXPECT_EQ(cmdDescr._type, CmdType::MOVE);
            MoveDescription expect{-1, -1};
            EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::MOVE>());
            EXPECT_EQ(cmdDescr.get_description<CmdType::MOVE>(), expect);

            UpdateUnitPosition(unit1, cmdDescr.get_description<CmdType::MOVE>(), map);
        }        
        com = unit1->process();
        {
            auto cmdDescr = com->execute();
            EXPECT_EQ(cmdDescr._type, CmdType::R_ATCK);
            RangeAttackDescription expect{0, rRange1, static_cast<hp_type>(agility)};
            EXPECT_NO_THROW(auto d = cmdDescr.get_description<CmdType::R_ATCK>());
            EXPECT_EQ(cmdDescr.get_description<CmdType::R_ATCK>(), expect);
        }
    }
}
}