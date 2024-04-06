#include <algorithm>
#include <gtest/gtest.h>
#include "../../src/Map/Map.hpp"
#include "../../src/Units/Unit.hpp"

namespace sw::map::test {
using namespace mngr::cmd;
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

TEST(command, exectest)
{
    units::UnitBuilder<units::UnitClass::WAR, units::Unit> warBuilder;
    warBuilder.add_action_by_priority(
            0,
            [](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::cout << "zero war action" << std::endl;
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                
                if (auto id = uRef.get_id(); id % 2)
                    ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MeleeAttackDescription>>(
                            uRef.get_id(), mngr::cmd::MeleeAttackDescription{1, 12});
                
                return ret;
            });
    warBuilder.add_action_by_priority(
            1,
            [](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::cout << "move war action" << std::endl;
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                
                ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MoveDescription>>(
                        uRef.get_id(), mngr::cmd::MoveDescription{1, 1});
                
                return ret;
            });
    
    units::UnitBuilder<units::UnitClass::ARCH, units::Unit> archBuilder;
    archBuilder.add_action_by_priority(
            0,
            [](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::cout << "zero arch action" << std::endl;
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                
                if (auto id = uRef.get_id(); id % 2)
                    ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::RangeAttackDescription>>(
                            uRef.get_id(),
                            mngr::cmd::RangeAttackDescription{1, 12, 123});
                
                return ret;
            });
    archBuilder.add_action_by_priority(
            1,
            [](units::IUnit &uRef, bool f) -> std::shared_ptr<mngr::cmd::IUnitCommand>
            {
                std::cout << "move arch action" << std::endl;
                std::shared_ptr<mngr::cmd::IUnitCommand> ret;
                ret = std::make_shared<mngr::cmd::UnitCommand<mngr::cmd::MoveDescription>>(
                        uRef.get_id(), mngr::cmd::MoveDescription{1, 1});
                
                return ret;
            });
    
    std::vector<std::unique_ptr<units::IUnit>> unitsOnMap;
    for (int i = 0; i < 10; ++i)
    {
        unitsOnMap.emplace_back(warBuilder.create_unit(i, 0));
        unitsOnMap.emplace_back(archBuilder.create_unit(i + 10, 0));
    }
    
    for (auto &unit: unitsOnMap)
    {
        if (auto cmd = unit->process())
        {
            auto res = cmd->execute();
            std::cout << res << std::endl;
        }
    }
}
}