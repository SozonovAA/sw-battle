#include <algorithm>
#include <gtest/gtest.h>
#include "../../src/Map/Map.hpp"
#include "../../src/Units/Unit.hpp"

namespace sw::map::test {
using UnitBuilderT = units::UnitBuilder<units::UnitClass::WAR, units::Unit>;
UnitBuilderT ub;
// Тест для метода addUnit: добавление юнита в свободную клетку
TEST(map, testAddUnitToEmptyCell)
{
    Map<units::IUnit> map(5, 5);
    EXPECT_EQ(map.addUnit(2, 3, ub.create_unit({0, 0})), 0); // Ожидаем успешное добавление
}

// Тест для метода addUnit: добавление юнита в занятую клетку
TEST(map, testAddUnitToOccupiedCell)
{
    Map<units::IUnit> map(5, 5);
    
    map.addUnit(2, 3, ub.create_unit({0, 0}));
    EXPECT_EQ(map.addUnit(2, 3, ub.create_unit({0, 0})), -2); // Ожидаем код ошибки -2 (клетка уже занята)
}

// Тест для метода getUnit: получение юнита из занятой клетки
TEST(map, testGetUnitFromOccupiedCell)
{
    Map<units::IUnit> map(5, 5);
    std::shared_ptr<units::IUnit> unit = ub.create_unit({0, 0});
    map.addUnit(2, 3, unit);
    std::shared_ptr<units::IUnit> retrievedUnit = map.getUnit(2, 3);
    EXPECT_EQ(retrievedUnit, unit); // Ожидаем получение того же самого юнита
}

// Тест для метода moveUnit: перемещение юнита в свободную клетку
TEST(map, testMoveUnitToEmptyCell)
{
    Map<units::IUnit> map(5, 5);
    
    map.addUnit(2, 3, ub.create_unit({0, 0}));
    EXPECT_EQ(map.moveUnit(2, 3, 3, 4), 0); // Ожидаем успешное перемещение
}

// Тест для метода moveUnit: перемещение юнита в занятую клетку
TEST(map, testMoveUnitToOccupiedCell)
{
    Map<units::IUnit> map(5, 5);
    std::shared_ptr<units::IUnit> unit1 = ub.create_unit({0, 0});
    std::shared_ptr<units::IUnit> unit2 = ub.create_unit({0, 0});
    map.addUnit(2, 3, unit1);
    map.addUnit(3, 4, unit2);
    EXPECT_EQ(map.moveUnit(2, 3, 3, 4), -3); // Ожидаем код ошибки -3 (клетка уже занята)
}

// Тест для метода getUnitsAround: получение объектов в радиусе от указанных координат
TEST(map, testGetUnitsAround)
{
    Map<units::IUnit> map(5, 5);
    const std::shared_ptr<units::IUnit> unit1 = ub.create_unit({1, 0});
    std::shared_ptr<units::IUnit> unit2 = ub.create_unit({2, 0});
    map.addUnit(2, 3, unit1);
    map.addUnit(3, 4, unit2);
    
    std::cout << map;
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
    // Создание карты размером 5x5
    Map<units::IUnit> map(5, 5);

    // Создание и добавление юнитов на карту
    const std::shared_ptr<units::IUnit> unit1 = ub.create_unit({1, 0});
    const std::shared_ptr<units::IUnit> unit2 = ub.create_unit({2, 0});
    const std::shared_ptr<units::IUnit> unit3 = ub.create_unit({3, 0});

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
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit3) != res.end());
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
    // Тесты для разных радиусов вокруг точек
    {
        const auto &res = map.getUnitsAround(2, 1, 1, 2);
        EXPECT_EQ(res.size(), 2);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit1) != res.end());
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit3) != res.end());
    }
    // Тесты для разных радиусов вокруг точек
    {
        const auto &res = map.getUnitsAround(2, 1, 3, 1000);
        EXPECT_EQ(res.size(), 1);
        EXPECT_TRUE(std::find(res.begin(), res.end(), unit2) != res.end());
    }
}

}