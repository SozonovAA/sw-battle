#pragma once

#include <iomanip>
#include <cmath>
#include "IMap.hpp"

namespace sw::map {

// Класс для хранения карты
    template<class EntityT>
    class Map : public IMap<EntityT>
    {
    public:
        using grid_type = std::vector<std::vector<std::shared_ptr<EntityT>>>;
        Map(unsigned rows, unsigned cols) : _rows(rows), _cols(cols)
        {
            _grid = grid_type(rows, std::vector<std::shared_ptr<EntityT>>(cols, nullptr));
        }
        
        int addUnit(unsigned x, unsigned y, std::shared_ptr<EntityT> unit) override
        {
            if (x >= _rows || y >= _cols)
                return -1;
            
            if (_grid[x][y])
                return -2;
            
            _grid[x][y] = unit;
            return 0;
        }
        
        std::shared_ptr<EntityT> getUnit(unsigned x, unsigned y) const override
        {
            if (x >= _rows || y >= _cols)
                return nullptr;
            
            return _grid[x][y];
        }
        
        int moveUnit(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY) override
        {
            if (fromX >= _rows || fromY >= _cols || toX >= _rows || toY >= _cols)
                return -1;
            
            if (!_grid[fromX][fromY])
                return -2;
            
            if (_grid[toX][toY])
                return -3;
            
            _grid[toX][toY] = std::move(_grid[fromX][fromY]);
            return 0;
        }
        
        // Метод для возвращения всех объектов в радиусе определенного количества клеток вокруг юнита
        std::vector<std::shared_ptr<EntityT>>
        getUnitsAround(const unsigned x, const unsigned y, const unsigned radius) const override
        {
            std::vector<std::shared_ptr<EntityT>> units;
            unsigned x_min = std::max(0, static_cast<int>(x - radius));
            unsigned x_max = std::min(_rows - 1, x + radius);
            unsigned y_min = std::max(0, static_cast<int>(y - radius));
            unsigned y_max = std::min(_cols - 1, y + radius);
            
            for (unsigned i = x_min; i <= x_max; ++i)
            {
                for (unsigned j = y_min; j <= y_max; ++j)
                {
                    if (!(x == i && y == j) && _grid[i][j])
                        units.push_back(_grid[i][j]);
                }
            }
            
            return units;
        }
        
        friend std::ostream &operator<<(std::ostream &os, const Map &map)
        {
            os << "---------------" << std::endl;
            for (int i = 0; i < map._rows; ++i)
            {
                for (int j = 0; j < map._cols; ++j)
                {
                    if (const auto entity = map._grid[j][i])
                    {
                        os << std::setw(2) << *entity;
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
        const unsigned _rows;
        const unsigned _cols;
        
        grid_type _grid;
    };
    
    
} // sw::map 