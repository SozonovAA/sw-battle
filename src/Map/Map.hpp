#pragma once

#include <iostream>
#include <iomanip>

#include <cmath>
namespace sw::map {
// Класс для хранения карты
template<class EntityT>
class Map {
public:
    Map(unsigned rows, unsigned cols) : rows_(rows), cols_(cols) {
        grid_ = std::vector<std::vector<std::shared_ptr<EntityT>>>(rows, std::vector<std::shared_ptr<EntityT>>(cols, nullptr));
    }
    
    int addUnit(unsigned x, unsigned y, std::shared_ptr<EntityT> unit) {
        if (x >= rows_ || y >= cols_)
            return -1;

        if (grid_[x][y])
            return -2;
        
        grid_[x][y] = unit;
        return 0;
    }

    std::shared_ptr<EntityT> getUnit(unsigned x, unsigned y) const {
        if (x >= rows_ || y >= cols_) 
            return nullptr;
        
        return grid_[x][y];
    }

    int moveUnit(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY) {
        if (fromX >= rows_ || fromY >= cols_ || toX >= rows_ || toY >= cols_)
            return -1;

        if (!grid_[fromX][fromY])
            return -2;

        if (grid_[toX][toY])
            return -3;

        grid_[toX][toY] = std::move(grid_[fromX][fromY]);
        return 0;
    }

    // Метод для возвращения всех объектов в радиусе определенного количества клеток вокруг юнита
    std::vector<std::shared_ptr<EntityT>> getUnitsAround(const unsigned x, const unsigned y, const unsigned radius) const {
        std::vector<std::shared_ptr<EntityT>> units;
        unsigned x_min = std::max(0, static_cast<int>(x - radius));
        unsigned x_max = std::min(rows_ - 1, x + radius);
        unsigned y_min = std::max(0, static_cast<int>(y - radius));
        unsigned y_max = std::min(cols_ - 1, y + radius);

        for (int i = x_min; i <= x_max; ++i) {
            for (int j = y_min; j <= y_max; ++j) {
                if (!(x == i && y == j) && grid_[i][j]) 
                    units.push_back(grid_[i][j]);
            }
        }

        return units;
    }

    friend std::ostream& operator<<(std::ostream& os, const Map& map) {
	    os << "---------------" << std::endl;
        for (int i = 0; i < map.rows_; ++i) 
        {
            for (int j = 0; j < map.cols_; ++j) 
            {
                if (const auto entity = map.grid_[j][i]) 
                {
                    os << std::setw(2)  << *entity;
                } else 
                {
                    os << std::setw(2) << ".";
                }
            }
            os << std::endl;
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