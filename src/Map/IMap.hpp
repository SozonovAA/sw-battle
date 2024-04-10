#pragma once

#include <cmath>
#include <memory>

namespace sw::map {
using coord_type = unsigned;
template<class EntityT>
class IMap
{
public:
    using entity_storage_type = std::shared_ptr<EntityT>;
    virtual int addUnit(coord_type x, coord_type y, entity_storage_type unit) = 0;
    virtual entity_storage_type getUnit(coord_type x, coord_type y) const = 0;
    virtual int moveUnit(coord_type fromX, coord_type fromY, coord_type toX, coord_type toY) = 0;

    virtual entity_storage_type deleteUnit(coord_type x, coord_type y) = 0;

    virtual std::vector<entity_storage_type>
    getUnitsAround(coord_type x, coord_type y, coord_type radius) const = 0;
    virtual std::vector<entity_storage_type>
    getUnitsAround(coord_type x, coord_type y, coord_type fromRadius, coord_type toRadius) const = 0;
    virtual bool isCoordinateCorrect(const coord_type& x, const coord_type& y) const noexcept =0;

    
    virtual ~IMap() = default;
};
static double getDistance(const coord_type& x, const coord_type& y, const coord_type& targetX, const coord_type& targetY)
{
    return std::sqrt(std::pow(static_cast<int>(targetX - x), 2) + std::pow(static_cast<int>(targetY - y), 2));

}
static bool checkRadius(const coord_type& x, const coord_type& y, const coord_type& targetX, const coord_type& targetY, const coord_type& R)
{
    return std::round(getDistance(x, y, targetX, targetY)) <= R;
}
}