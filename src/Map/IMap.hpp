#pragma once

#include <memory>

namespace sw::map {
template<class EntityT>
class IMap
{
    public:
    using entity_storage_type = std::shared_ptr<EntityT>;
    virtual int addUnit(unsigned x, unsigned y, entity_storage_type unit) = 0;
    virtual entity_storage_type getUnit(unsigned x, unsigned y) const = 0;
    virtual int moveUnit(unsigned fromX, unsigned fromY, unsigned toX, unsigned toY) = 0;
    virtual std::vector<entity_storage_type> getUnitsAround(const unsigned x, const unsigned y, const unsigned radius) const = 0;

    virtual ~IMap() = default;
};
}