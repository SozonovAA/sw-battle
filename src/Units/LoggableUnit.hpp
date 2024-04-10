#pragma once
#include "Unit.hpp"

#include "../IO/EventLogs/MarchStarted.hpp"
#include "../IO/EventLogs/MarchEnded.hpp"
#include "../IO/System/PrintDebug.hpp"
#include "../IO/System/EventLog.hpp"

namespace sw::units {
class LoggableUnit : public Unit
{
    public:
    LoggableUnit(UnitClass type, const UnitDescription& ud) : Unit(type, ud)
    {
        registerEvents();
    }
    LoggableUnit(UnitClass type, id_type id, hp_type hp, param_type step_count) : Unit(type, id, hp, step_count)
    {
        registerEvents();
    }

    LoggableUnit(const LoggableUnit &) = default;
    LoggableUnit(LoggableUnit &&) = default;
    LoggableUnit &operator=(const LoggableUnit &) = delete;
    LoggableUnit &operator=(LoggableUnit &&) = delete;
    ~LoggableUnit() override = default;


    std::shared_ptr<mngr::cmd::IUnitCommand> process() override
    {
        if(_march_coord.has_value() && _coord == _march_coord)
        {
                _eventLog.log(io::MarchEnded{ 
                    getId(), 
                    static_cast<uint32_t>(_coord._x), 
                    static_cast<uint32_t>(_coord._y)})
                ;
        }
        return Unit::process();
    }
    void setMarchPosition(const map::Point &aim) override
    {
        Unit::setMarchPosition(aim);
        _eventLog.log(io::MarchStarted{ 
            getId(), 
            static_cast<uint32_t>(_coord._x), 
            static_cast<uint32_t>(_coord._y), 
            static_cast<uint32_t>(_march_coord.value()._x), 
            static_cast<uint32_t>(_march_coord.value()._y)}
        );
    }

    private:

    EventLog _eventLog;

    void registerEvents()
    {
        _eventLog.listen<sw::io::MarchStarted>([](auto& event){ printDebug(std::cout, event); });
        _eventLog.listen<sw::io::MarchEnded>([](auto& event){ printDebug(std::cout, event); });
    }
};

}