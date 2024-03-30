#include <memory>
namespace sw::units {
class IUnit
{
    public:
    virtual void update() = 0;
    virtual std::unique_ptr<IUnit> clone() const = 0;
    virtual ~IUnit() = default;
};
}