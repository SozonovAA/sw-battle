#pragma once

#include <iomanip>

#include "../Units/IUnit.hpp"

namespace sw::map {

// Класс для хранения карты
template<class EntityT>
class Map {
public:
    Map(unsigned rows, unsigned cols) : rows_(rows), cols_(cols) {
        grid_ = std::vector<std::vector<std::shared_ptr<EntityT>>>(rows, std::vector<std::shared_ptr<EntityT>>(cols, nullptr));
    }

    // Метод для добавления объекта на карту по указанным координатам
    int addUnit(unsigned x, unsigned y, std::shared_ptr<EntityT> unit) {
        if (x >= rows_ || y >= cols_) {
            return -1; // Код ошибки: неверные координаты
        }

        if (grid_[x][y]) {
            return -2; // Код ошибки: ячейка уже занята
        }

        grid_[x][y] = unit;
        return 0; // Успешное добавление
    }

    // Метод для получения копии юнита по заданным координатам
    std::shared_ptr<EntityT> getUnit(int x, int y) const {
        if (x < 0 || x >= rows_ || y < 0 || y >= cols_) {
            return nullptr; // Возвращаем nullptr, если координаты неверные
        }

        return grid_[x][y]; // Возвращаем указатель на юнит в заданных координатах
    }


    // Перегрузка оператора вывода для вывода карты в поток
    friend std::ostream& operator<<(std::ostream& os, const Map& map) {
	    os << "---------------" << std::endl;
        for (int i = 0; i < map.rows_; ++i) {
            for (int j = 0; j < map.cols_; ++j) {
                if (const auto entity = map.grid_[i][j]) {
                    os << std::setw(2)  << *entity; // Если ячейка занята, выводим "X"
                } else {
                    os << std::setw(2) << "."; // Если ячейка пуста, выводим "."
                }
            }
            os << std::endl; // Разделяем строки
        }
	    os << "---------------" << std::endl;
        return os;
    }
private:
    const unsigned rows_;
    const unsigned cols_;
    std::vector<std::vector<std::shared_ptr<EntityT>>> grid_;
};
} // sw::map 