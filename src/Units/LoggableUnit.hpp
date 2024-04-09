#pragma once
#include "Unit.hpp"

#include "../IO/EventLogs/MarchStarted.hpp"
#include "../IO/EventLogs/MarchEnded.hpp"
#include "../IO/System/PrintDebug.hpp"
#include "../IO/System/EventLog.hpp"
#include <_types/_uint32_t.h>

namespace sw::units {
class LoggableUnit : public Unit
{
    public:
    LoggableUnit(UnitClass type, const UnitDescription& ud) : Unit(type, ud)
    {
        register_events();
    }
    LoggableUnit(UnitClass type, id_type id, hp_type hp, param_type step_count) : Unit(type, id, hp, step_count)
    {
        register_events();
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
                    get_id(), 
                    static_cast<uint32_t>(_coord._x), 
                    static_cast<uint32_t>(_coord._y)})
                ;
        }
        return Unit::process();
    }
    void set_march_position(const map::Point &aim) override
    {
        Unit::set_march_position(aim);
        _eventLog.log(io::MarchStarted{ 
            get_id(), 
            static_cast<uint32_t>(_coord._x), 
            static_cast<uint32_t>(_coord._y), 
            static_cast<uint32_t>(_march_coord.value()._x), 
            static_cast<uint32_t>(_march_coord.value()._x)}
        );
    }

    private:

    EventLog _eventLog;

    void register_events()
    {
        _eventLog.listen<sw::io::MarchStarted>([](auto& event){ printDebug(std::cout, event); });
        _eventLog.listen<sw::io::MarchEnded>([](auto& event){ printDebug(std::cout, event); });
    }
};

}