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
        EXPECT_EQ(res.getDescription<CmdType::M_ATCK>(), descr);
    }
    {
        auto descr = MoveDescription{1, 12};
        auto ret = std::make_shared<UnitCommand<MoveDescription>>(1, descr);
        auto res = ret->execute();
        EXPECT_EQ(res._id, 1);
        EXPECT_TRUE(res._type == CmdType::MOVE);
        EXPECT_EQ(res.getDescription<CmdType::MOVE>(), descr);
    }
    {
        auto descr = RangeAttackDescription{1, 1, 123};
        auto ret = std::make_shared<UnitCommand<RangeAttackDescription>>(2, descr);
        auto res = ret->execute();
        EXPECT_EQ(res._id, 2);
        EXPECT_TRUE(res._type == CmdType::R_ATCK);
        EXPECT_EQ(res.getDescription<CmdType::R_ATCK>(), descr);
    }
}

TEST(command, defaultMarchTest)
{
    units::UnitBuilder<units::UnitClass::WAR, units::Unit> warBuilder;
    const auto maDescr = mngr::cmd::MeleeAttackDescription{1, 12};
    const auto mvDescr = mngr::cmd::MoveDescription{1, 12};
    const auto raDescr = mngr::cmd::RangeAttackDescription{1, 12, 123};

    auto unit = warBuilder.createUnit({0, 0});
    unit->setUnitPosition({5, 5});
    {
        unit->setMarchPosition({7, 7});
        const Point expectPoint (1, 1);
        const auto moveCommand = defaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().getDescription<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->setMarchPosition({4, 4});
        const Point expectPoint (-1, -1);
        const auto moveCommand = defaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().getDescription<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }   
    {
        unit->setMarchPosition({5, 5});
        const Point expectPoint (0, 0);
        const auto moveCommand = defaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().getDescription<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->setMarchPosition({4, 5});
        const Point expectPoint (-1, 0);
        const auto moveCommand = defaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().getDescription<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->setMarchPosition({5, 4});
        const Point expectPoint (0, -1);
        const auto moveCommand = defaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().getDescription<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }

    unit->setUnitPosition({-5, -5});
    {
        unit->setMarchPosition({5, 4});
        const Point expectPoint (1, 1);
        const auto moveCommand = defaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().getDescription<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->setMarchPosition({-6, -6});
        const Point expectPoint (-1, -1);
        const auto moveCommand = defaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().getDescription<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
    {
        unit->setMarchPosition({1000, -6});
        const Point expectPoint (1, -1);
        const auto moveCommand = defaultMarchMethod(unit);
        const auto moveDescr = moveCommand->execute().getDescription<CmdType::MOVE>();
        EXPECT_EQ(moveDescr.delta_x, expectPoint._x);
        EXPECT_EQ(moveDescr.delta_y, expectPoint._y);
    }
}

TEST(command, cmdPriorityTest)
{
    using namespace units;
    units::UnitBuilder<units::UnitClass::UNDEF, units::Unit> warBuilder;
    const auto maDescr = mngr::cmd::MeleeAttackDescription{1, 12};
    const auto mvDescr = mngr::cmd::MoveDescription{1, 12};
    const auto raDescr = mngr::cmd::RangeAttackDescription{1, 12, 123};
    warBuilder.addActionByPriority(
            0,
            [maDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                
                if (auto id = uRef->getId(); id == 0)
                    ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MeleeAttackDescription>>(
                            uRef->getId(), maDescr);
                
                return ret;
            });
    warBuilder.addActionByPriority(
        1,
        [raDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
        {
            std::shared_ptr<mngr::cmd::IUnitCommand> ret;
            
            if (auto id = uRef->getId(); id == 1)
                ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::RangeAttackDescription>>(
                        uRef->getId(), raDescr);
            
            return ret;
        }
    );
    warBuilder.setMarchMethod(
        [](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
        {
            return defaultMarchMethod(uRef);
        }
    );
    {
        const auto cmd = warBuilder.createUnit({0, 0})->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::M_ATCK);
        EXPECT_NO_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::M_ATCK>());
        EXPECT_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::MOVE>(), std::bad_any_cast);
        EXPECT_EQ(cmd.getDescription<mngr::cmd::CmdType::M_ATCK>(), maDescr);
    }
    {
        const auto cmd = warBuilder.createUnit({1, 0})->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::R_ATCK);
        EXPECT_NO_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::R_ATCK>());
        EXPECT_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::MOVE>(), std::bad_any_cast);
        EXPECT_EQ(cmd.getDescription<mngr::cmd::CmdType::R_ATCK>(), raDescr);
    }
    {
        auto unit = warBuilder.createUnit({2, 0});
        unit->setUnitPosition({5, 5});
        unit->setMarchPosition({7, 7});
        const auto cmd = unit->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::MOVE);
        EXPECT_NO_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::MOVE>());
        EXPECT_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::R_ATCK>(), std::bad_any_cast);
        const auto mvDescr =  MoveDescription{1, 1}; 
        EXPECT_EQ(cmd.getDescription<mngr::cmd::CmdType::MOVE>(), mvDescr);
    }


    units::UnitBuilder<units::UnitClass::ARCH, units::Unit> archBuilder;
    archBuilder.addActionByPriority(
            10,
            [raDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                
                if (auto id = uRef->getId(); id % 2)
                    ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::RangeAttackDescription>>(
                            uRef->getId(),
                            raDescr);
                
                return ret;
            });
    archBuilder.addActionByPriority(
            1,
            [mvDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;

                if (auto id = uRef->getId(); id % 2 && id < 10)
                    ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MoveDescription>>(
                            uRef->getId(),
                            mvDescr);
                            
                
                return ret;
            });
    archBuilder.addActionByPriority(
            1000,
            [maDescr](std::shared_ptr<IUnit> uRef) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MeleeAttackDescription>>(
                        uRef->getId(), maDescr);
                
                return ret;
            });

    {
        const auto cmd = archBuilder.createUnit({1, 0})->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::MOVE);
        EXPECT_NO_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::MOVE>());
        EXPECT_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::R_ATCK>(), std::bad_any_cast);
        EXPECT_EQ(cmd.getDescription<mngr::cmd::CmdType::MOVE>(), mvDescr);
    }
    {
        const auto cmd = archBuilder.createUnit({11, 0})->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::R_ATCK);
        EXPECT_NO_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::R_ATCK>());
        EXPECT_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::MOVE>(), std::bad_any_cast);
        EXPECT_EQ(cmd.getDescription<mngr::cmd::CmdType::R_ATCK>(), raDescr);
    }
    {
        const auto cmd = archBuilder.createUnit({6, 0})->process()->execute();
        EXPECT_EQ(cmd._type, mngr::cmd::CmdType::M_ATCK);
        EXPECT_NO_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::M_ATCK>());
        EXPECT_THROW(auto i = cmd.getDescription<mngr::cmd::CmdType::MOVE>(), std::bad_any_cast);
        EXPECT_EQ(cmd.getDescription<mngr::cmd::CmdType::M_ATCK>(), maDescr);
    }
}
}