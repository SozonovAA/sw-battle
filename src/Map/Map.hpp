#pragma once

#include <iomanip>
#include <cmath>
#include "IMap.hpp"

namespace sw::map {

template<class EntityT>
class Map : public IMap<EntityT>
{
public:
    using entity_type = EntityT;
    using grid_type = std::vector<std::vector<std::shared_ptr<EntityT>>>;

    Map(coord_type rows, coord_type cols) : _rows(rows), _cols(cols)
    {
        _grid = grid_type(rows, std::vector<std::shared_ptr<EntityT>>(cols, nullptr));
    }
    
    int addUnit(coord_type x, coord_type y, std::shared_ptr<EntityT> unit) override
    {
        if (x >= _rows || y >= _cols)
            return -1;
        
        if (_grid[x][y])
            return -2;
        
        _grid[x][y] = unit;
        return 0;
    }
    
    std::shared_ptr<EntityT> getUnit(coord_type x, coord_type y) const override
    {
        if (x >= _rows || y >= _cols)
            return nullptr;
        
        return _grid[x][y];
    }
    
    int moveUnit(coord_type fromX, coord_type fromY, coord_type toX, coord_type toY) override
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


    std::shared_ptr<EntityT> deleteUnit(coord_type x, coord_type y) override
    {
        if (x >= _rows || y >= _cols)
            return {};
        
        auto res = _grid[x][y];
        _grid[x][y] = nullptr;
        return res;
    }
    
    std::vector<std::shared_ptr<EntityT>>
    getUnitsAround(coord_type x, coord_type y, coord_type radius) const override
    {
        std::vector<std::shared_ptr<EntityT>> units;
        coord_type x_min = std::max(0, static_cast<int>(x - radius));
        coord_type x_max = std::min(_rows - 1, x + radius);
        coord_type y_min = std::max(0, static_cast<int>(y - radius));
        coord_type y_max = std::min(_cols - 1, y + radius);
        
        for (coord_type i = x_min; i <= x_max; ++i)
        {
            for (coord_type j = y_min; j <= y_max; ++j)
            {
                if (!(x == i && y == j) && _grid[i][j] && checkRadius(x, y, i, j, radius))
                    units.push_back(_grid[i][j]);
            }
        }
        
        return units;
    }
    
    std::vector<std::shared_ptr<EntityT>>
    getUnitsAround(coord_type x, coord_type y, coord_type fromRadius, coord_type toRadius) const override
    {
        std::vector<std::shared_ptr<EntityT>> units;
        if (fromRadius > toRadius || toRadius <= 0)
            return units;

        coord_type x_min = std::max(0, static_cast<int>(x - toRadius));
        coord_type x_max = std::min(_rows - 1, x + toRadius);
        coord_type y_min = std::max(0, static_cast<int>(y - toRadius));
        coord_type y_max = std::min(_cols - 1, y + toRadius);

        for (coord_type i = x_min; i <= x_max; ++i)
        {
            for (coord_type j = y_min; j <= y_max; ++j)
            {
                int distance_x = abs(static_cast<int>(i) - static_cast<int>(x)); 
                int distance_y = abs(static_cast<int>(j) - static_cast<int>(y));
                if (_grid[i][j] && !(x == i && y == j) && 
                    (distance_x >= fromRadius || distance_y >= fromRadius) && 
                    checkRadius(x, y, i, j, toRadius)
                )
                {
                    units.push_back(_grid[i][j]);
                }
            }
        }
        return units;
    }

    bool isCoordinateCorrect(const coord_type& x, const coord_type& y) const noexcept override
    {
        return !(x >= _rows || y >= _cols);
    }
    
protected:
    const coord_type _rows;
    const coord_type _cols;
    
    grid_type _grid;
};
    
} // sw::map 