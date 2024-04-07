#include <algorithm>
#include <gtest/gtest.h>
#include <stdexcept>
#include "../../src/Map/Map.hpp"
#include "../../src/Units/Unit.hpp"
#include "../../src/Units/BuilderTemplates/DefaultFunctions.hpp"

namespace sw::mngr::cmd::test {
using namespace map;
using namespace units::templates;
TEST(command, testExecute)
{
    {
        auto descr = MeleeAttackDescription{1, 12};
        auto ret = std::make_shared<UnitCommand<MeleeAttackDescription>>(0, descr);
        auto res = ret->execute();
        EXPECT_EQ(res._id, 0);
        EXPECT_TRUE(res._type == CmdType::M_ATCK);
        EXPECT_EQ(res.get_description<CmdType::M_ATCK>(), descr);
    }
    {
        auto descr = MoveDescription{1, 12};
        auto ret = std::make_shared<UnitCommand<MoveDescription>>(1, descr);
        auto res = ret->execute();
        EXPECT_EQ(res._id, 1);
        EXPECT_TRUE(res._type == CmdType::MOVE);
        EXPECT_EQ(res.get_description<CmdType::MOVE>(), descr);
    }
    {
        auto descr = RangeAttackDescription{1, 1, 123};
        auto ret = std::make_shared<UnitCommand<RangeAttackDescription>>(2, descr);
        auto res = ret->execute();
        EXPECT_EQ(res._id, 2);
        EXPECT_TRUE(res._type == CmdType::R_ATCK);
        EXPECT_EQ(res.get_description<CmdType::R_ATCK>(), descr);
    }
}

TEST(command, defaultMarchTest)
{
    units::UnitBuilder<units::UnitClass::WAR, units::Unit> warBuilder;
    const auto maDescr = mngr::cmd::MeleeAttackDescription{1, 12};
    const auto mvDescr = mngr::cmd::MoveDescription{1, 12};
    const auto raDescr = mngr::cmd::RangeAttackDescription{1, 12, 123};

    auto unit = warBuilder.create_unit(0, 0);
    unit->set_unit_position({5, 5});
    {
        unit->set_march_position({7, 7});
        const Point expectPoint (1, 1);
        const auto moveCommand = DefaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().get_description<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->set_march_position({4, 4});
        const Point expectPoint (-1, -1);
        const auto moveCommand = DefaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().get_description<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }   
    {
        unit->set_march_position({5, 5});
        const Point expectPoint (0, 0);
        const auto moveCommand = DefaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().get_description<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->set_march_position({4, 5});
        const Point expectPoint (-1, 0);
        const auto moveCommand = DefaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().get_description<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->set_march_position({5, 4});
        const Point expectPoint (0, -1);
        const auto moveCommand = DefaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().get_description<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }

    unit->set_unit_position({-5, -5});
    {
        unit->set_march_position({5, 4});
        const Point expectPoint (1, 1);
        const auto moveCommand = DefaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().get_description<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->set_march_position({-6, -6});
        const Point expectPoint (-1, -1);
        const auto moveCommand = DefaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().get_description<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->set_march_position({1000, -6});
        const Point expectPoint (1, -1);
        const auto moveCommand = DefaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().get_description<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
}

TEST(command, cmdPriorityTest)
{
    using namespace units;
    units::UnitBuilder<units::UnitClass::WAR, units::Unit> warBuilder;
    const auto maDescr = mngr::cmd::MeleeAttackDescription{1, 12};
    const auto mvDescr = mngr::cmd::MoveDescription{1, 12};
    const auto raDescr = mngr::cmd::RangeAttackDescription{1, 12, 123};
    warBuilder.add_action_by_priority(
            0,
            [maDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                
                if (auto id = uRef->get_id(); id == 0)
                    ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MeleeAttackDescription>>(
                            uRef->get_id(), maDescr);
                
                return ret;
            });
    warBuilder.add_action_by_priority(
        1,
        [raDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
        {
            std::shared_ptr<mngr::cmd::IUnitCommand> ret;
            
            if (auto id = uRef->get_id(); id == 1)
                ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::RangeAttackDescription>>(
                        uRef->get_id(), raDescr);
            
            return ret;
        }
    );
    warBuilder.set_march_method(
        [](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
        {
            return DefaultMarchMethod(uRef);
        }
    );
    {
        const auto cmd = warBuilder.create_unit(0, 0)->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::M_ATCK);
        EXPECT_NO_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::M_ATCK>());
        EXPECT_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::MOVE>(), std::bad_any_cast);
        EXPECT_EQ(cmd.get_description<mngr::cmd::CmdType::M_ATCK>(), maDescr);
    }
    {
        const auto cmd = warBuilder.create_unit(1, 0)->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::R_ATCK);
        EXPECT_NO_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::R_ATCK>());
        EXPECT_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::MOVE>(), std::bad_any_cast);
        EXPECT_EQ(cmd.get_description<mngr::cmd::CmdType::R_ATCK>(), raDescr);
    }
    {
        auto unit = warBuilder.create_unit(2, 0);
        unit->set_unit_position({5, 5});
        unit->set_march_position({7, 7});
        const auto cmd = unit->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::MOVE);
        EXPECT_NO_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::MOVE>());
        EXPECT_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::R_ATCK>(), std::bad_any_cast);
        const auto mvDescr =  MoveDescription{1, 1}; 
        EXPECT_EQ(cmd.get_description<mngr::cmd::CmdType::MOVE>(), mvDescr);
    }


    units::UnitBuilder<units::UnitClass::ARCH, units::Unit> archBuilder;
    archBuilder.add_action_by_priority(
            10,
            [raDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                
                if (auto id = uRef->get_id(); id % 2)
                    ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::RangeAttackDescription>>(
                            uRef->get_id(),
                            raDescr);
                
                return ret;
            });
    archBuilder.add_action_by_priority(
            1,
            [mvDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;

                if (auto id = uRef->get_id(); id % 2 && id < 10)
                    ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MoveDescription>>(
                            uRef->get_id(),
                            mvDescr);
                            
                
                return ret;
            });
    archBuilder.add_action_by_priority(
            1000,
            [maDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MeleeAttackDescription>>(
                        uRef->get_id(), maDescr);
                
                return ret;
            });

    {
        const auto cmd = archBuilder.create_unit(1, 0)->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::MOVE);
        EXPECT_NO_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::MOVE>());
        EXPECT_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::R_ATCK>(), std::bad_any_cast);
        EXPECT_EQ(cmd.get_description<mngr::cmd::CmdType::MOVE>(), mvDescr);
    }
    {
        const auto cmd = archBuilder.create_unit(11, 0)->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::R_ATCK);
        EXPECT_NO_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::R_ATCK>());
        EXPECT_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::MOVE>(), std::bad_any_cast);
        EXPECT_EQ(cmd.get_description<mngr::cmd::CmdType::R_ATCK>(), raDescr);
    }
    {
        const auto cmd = archBuilder.create_unit(6, 0)->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::M_ATCK);
        EXPECT_NO_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::M_ATCK>());
        EXPECT_THROW(auto i = cmd.get_description<mngr::cmd::CmdType::MOVE>(), std::bad_any_cast);
        EXPECT_EQ(cmd.get_description<mngr::cmd::CmdType::M_ATCK>(), maDescr);
    }
}
}