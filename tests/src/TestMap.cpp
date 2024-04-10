#include <algorithm>
#include <gtest/gtest.h>
#include "../../src/Map/LoggableMap.hpp"
#include "../../src/Units/Unit.hpp"

namespace sw::map::test {
using UnitBuilderT = units::UnitBuilder<units::UnitClass::WAR, units::Unit>;
UnitBuilderT ub;
TEST(map, check_radiusTest)
{
    EXPECT_EQ(checkRadius(0, 0, 2, 2, 2), false);
    
    EXPECT_EQ(checkRadius(0, 0, 3, 4, 5), true);
    EXPECT_EQ(checkRadius(0, 0, 5, 0, 5), true);
    EXPECT_EQ(checkRadius(0, 0, 0, 0, 5), true);

    EXPECT_EQ(checkRadius(0, 0, 0, 5, 5), true);
    EXPECT_EQ(checkRadius(0, 0, 5, 0, 5), true);

    EXPECT_EQ(checkRadius(0, 0, 6, 0, 5), false);
    EXPECT_EQ(checkRadius(0, 0, 3, 4, 3), false);
}

TEST(map, testAddUnitToEmptyCell)
{
    Map<units::IUnit> map(5, 5);
    EXPECT_EQ(map.addUnit(2, 3, ub.createUnit({0, 0})), 0); 
}

TEST(map, testAddUnitToOccupiedCell)
{
    Map<units::IUnit> map(5, 5);
    
    map.addUnit(2, 3, ub.createUnit({0, 0}));
    EXPECT_EQ(map.addUnit(2, 3, ub.createUnit({0, 0})), -2); 
}

TEST(map, testGetUnitFromOccupiedCell)
{
    Map<units::IUnit> map(5, 5);
    std::shared_ptr<units::IUnit> unit = ub.createUnit({0, 0});
    map.addUnit(2, 3, unit);
    std::shared_ptr<units::IUnit> retrievedUnit = map.getUnit(2, 3);
    EXPECT_EQ(retrievedUnit, unit); 
}

TEST(map, testMoveUnitToEmptyCell)
{
    Map<units::IUnit> map(5, 5);
    
    map.addUnit(2, 3, ub.createUnit({0, 0}));
    EXPECT_EQ(map.moveUnit(2, 3, 3, 4), 0);
}

TEST(map, testMoveUnitToOccupiedCell)
{
    Map<units::IUnit> map(5, 5);
    std::shared_ptr<units::IUnit> unit1 = ub.createUnit({0, 0});
    std::shared_ptr<units::IUnit> unit2 = ub.createUnit({0, 0});
    map.addUnit(2, 3, unit1);
    map.addUnit(3, 4, unit2);
    EXPECT_EQ(map.moveUnit(2, 3, 3, 4), -3);
}

TEST(map, testGetUnitsAround)
{
    Map<units::IUnit> map(5, 5);
    const std::shared_ptr<units::IUnit> unit1 = ub.createUnit({1, 0});
    std::shared_ptr<units::IUnit> unit2 = ub.createUnit({2, 0});
    map.addUnit(2, 3, unit1);
    map.addUnit(3, 4, unit2);
    
    {
        const auto &res = map.getUnitsAround(3, 3, 1);
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(3, 3, 2);
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(4, 4, 1);
        EXPECT_EQ(res.size(), 1);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) == res.end());
    }
    {
        const auto &res = map.getUnitsAround(4, 4, 5);
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(4, 4, 0);
        EXPECT_EQ(res.size(), 0);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) == res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) == res.end());
    }
    {
        const auto &res = map.getUnitsAround(2, 3, 0);
        EXPECT_EQ(res.size(), 0);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) == res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) == res.end());
    }
}

TEST(map, testGetUnitsAroundFromTo)
{
    LoggableMap<units::IUnit> map(5, 5);
    const std::shared_ptr<units::IUnit> unit1 = ub.createUnit({1, 0});
    const std::shared_ptr<units::IUnit> unit2 = ub.createUnit({2, 0});
    const std::shared_ptr<units::IUnit> unit3 = ub.createUnit({3, 0});

    map.addUnit(2, 3, unit1);
    map.addUnit(3, 4, unit2);
    map.addUnit(0, 0, unit3);

    // Тесты для разных радиусов вокруг точек
    {
        const auto &res = map.getUnitsAround(3, 3, 0, 1);
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(3, 3, 1, 2);
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(2, 2, 2, 2);
        EXPECT_EQ(res.size(), 1);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(3, 3, 1, 1);
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(3, 3, 1, 1000);
        EXPECT_EQ(res.size(), 3);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit3) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(3, 3, 2, 1000);
        EXPECT_EQ(res.size(), 1);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit3) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(2, 1, 1, 2);
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit3) != res.end());
    }
    {
        const auto &res = map.getUnitsAround(2, 1, 3, 1000);
        EXPECT_EQ(res.size(), 1);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
}

}