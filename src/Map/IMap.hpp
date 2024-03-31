#pragma once

#include <memory>

namespace sw::map {
template<class EntityT>
class IMap
{
    public:
    virtual int addUnit(unsigned x, unsigned y, std::shared_ptr<EntityT> unit) = 0;
    virtual std::shared_ptr<EntityT> getUnit(unsigned x, unsigned y) const = 0;
    virtual int moveUnit(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY) = 0;
    virtual std::vector<std::shared_ptr<EntityT>> getUnitsAround(const unsigned x, const unsigned y, const unsigned radius) const = 0;

    virtual ~IMap() = default;
};
}