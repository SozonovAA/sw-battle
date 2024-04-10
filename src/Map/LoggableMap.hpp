#pragma once
#include <ostream>
#include "Map.hpp"


#include "../IO/EventLogs/MapCreated.hpp"
#include "../IO/System/PrintDebug.hpp"
#include "../IO/System/EventLog.hpp"

namespace sw::map {
// Класс для хранения карты
template<class EntityT>
class LoggableMap : public Map<EntityT>
{
    public:
    LoggableMap(unsigned rows, unsigned cols) : Map<EntityT>(rows, cols) 
    {
        EventLog eventLog;
        eventLog.listen<io::MapCreated>([](auto& event){ printDebug(std::cout, event); });
        eventLog.log(io::MapCreated{ rows, cols });
    };

    friend std::ostream &operator<<(std::ostream &os, const LoggableMap &map)
    {
        os << "---------------" << std::endl;
        os << " #  ";
        for (int j = 0; j < map._cols; ++j)
            os << j << std::setw(10);
        
        os << std::endl;
        for (int i = 0; i < map._rows; ++i)
        {
            os << std::setw(4) << i;
            for (int j = 0; j < map._cols; ++j)
            {
                if (const auto entity = map._grid[j][i])
                {
                    os << "|" << *entity << "|";
                } else
                {
                    os << "|________|";
                    //os << "." << std::setw(7);
                }
            }
            os << std::endl;
        }
        os << "---------------" << std::endl;
        return os;
    }
};
}