#include <gtest/gtest.h>
#include "../../src/Map/Map.hpp"
#include "../../src/Units/Unit.hpp"


namespace sw::map::test {
using UnitBuilderT = units::UnitBuilder<units::UnitClass::WAR, units::Unit>;
UnitBuilderT ub; 
// Тест для метода addUnit: добавление юнита в свободную клетку
TEST(map, testAddUnitToEmptyCell){
    Map<units::IUnit> map(5, 5);
    EXPECT_EQ(map.addUnit(2, 3, ub.create_unit(0)), 0); // Ожидаем успешное добавление
}

// Тест для метода addUnit: добавление юнита в занятую клетку
TEST(map, testAddUnitToOccupiedCell){
    Map<units::IUnit> map(5, 5);
    
    map.addUnit(2, 3, ub.create_unit(0));
    EXPECT_EQ(map.addUnit(2, 3, ub.create_unit(0)), -2); // Ожидаем код ошибки -2 (клетка уже занята)
}

// Тест для метода getUnit: получение юнита из занятой клетки
TEST(map, testGetUnitFromOccupiedCell){
    Map<units::IUnit> map(5, 5);
    std::shared_ptr<units::IUnit> unit = ub.create_unit(0);
    map.addUnit(2, 3, unit);
    std::shared_ptr<units::IUnit> retrievedUnit = map.getUnit(2, 3);
    EXPECT_EQ(retrievedUnit, unit); // Ожидаем получение того же самого юнита
}

// Тест для метода moveUnit: перемещение юнита в свободную клетку
TEST(map, testMoveUnitToEmptyCell){
    Map<units::IUnit> map(5, 5);
    
    map.addUnit(2, 3, ub.create_unit(0));
    EXPECT_EQ(map.moveUnit(2, 3, 3, 4), 0); // Ожидаем успешное перемещение
}

// Тест для метода moveUnit: перемещение юнита в занятую клетку
TEST(map, testMoveUnitToOccupiedCell){
    Map<units::IUnit> map(5, 5);
    std::shared_ptr<units::IUnit> unit1 = ub.create_unit(0);
    std::shared_ptr<units::IUnit> unit2 = ub.create_unit(0);
    map.addUnit(2, 3, unit1);
    map.addUnit(3, 4, unit2);
    EXPECT_EQ(map.moveUnit(2, 3, 3, 4), -3); // Ожидаем код ошибки -3 (клетка уже занята)
}

// Тест для метода getUnitsAround: получение объектов в радиусе от указанных координат
TEST(map, testGetUnitsAround){
    Map<units::IUnit> map(5, 5);
    std::shared_ptr<units::IUnit> unit1 = ub.create_unit(0);
    std::shared_ptr<units::IUnit> unit2 = ub.create_unit(0);
    map.addUnit(2, 3, unit1);
    map.addUnit(3, 4, unit2);

    std::cout << map;

    //todo : fix
    // Проверяем, что в радиусе 1 от (2,3) находится unit2, а в радиусе 2 - unit1 и unit2
    EXPECT_EQ(map.getUnitsAround(2, 3, 1).size(), 1);
    EXPECT_EQ(map.getUnitsAround(2, 3, 1)[0], unit2);

    EXPECT_EQ(map.getUnitsAround(3, 3, 2).size(), 2);
    EXPECT_EQ(map.getUnitsAround(2, 3, 2)[0], unit1);
    EXPECT_EQ(map.getUnitsAround(2, 3, 2)[1], unit2);
}
}