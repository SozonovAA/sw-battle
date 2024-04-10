
#include <IO/System/CommandParser.hpp>
#include <IO/System/PrintDebug.hpp>
#include <exception>
#include <iostream>
#include <fstream>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/Wait.hpp>
#include <IO/Commands/WaitPrintMap.hpp>
#include <memory>

#include "GameManager/GameManager.hpp"
#include "GameManager/Executors/Default/GameExecutor.hpp"
#include "GameManager/Executors/Loggable/LoggableGameExecutor.hpp"
#include "Map/Coords.hpp"
#include "Units/LoggableUnit.hpp"
#include "Units/IUnit.hpp"
#include "Map/Map.hpp"
#include "Map/LoggableMap.hpp"
#include "Units/BuilderTemplates/DefaultUnitsBuilders.hpp"
#include "Units/UnitTypes.hpp"



int main(int argc, char **argv)
{
    using namespace sw;
    using namespace sw::mngr;
    using namespace sw::map;
    using namespace sw::units;
    
    
    if (argc != 2) {
    	throw std::runtime_error("Error: No file specified in command line argument");
    }
    
    std::ifstream file(argv[1]);
    if (!file) {
    	throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
    }

    try {
        GameManager gm;
        io::CommandParser parser;
        std::shared_ptr<LoggableMap<units::IUnit>> map;
        parser.add<io::CreateMap>(
            [&gm, &map](auto command) mutable
            {
                map = gm.createMap<LoggableMap<units::IUnit>, LoggableGameExecutor>(command.height, command.width);
            }).add<io::SpawnWarrior>(
            [&gm](auto command)
            {
                UnitDescription ud1{command.unitId, static_cast<hp_type>(command.hp)};
                Point coord{static_cast<Point::coord_type>(command.x), static_cast<Point::coord_type>(command.y)};
                WarriorDescription descr{command.strength, 1};
                gm.spawnUnit<LoggableUnit>(descr, ud1, coord);
            }).add<io::SpawnArcher>(
            [&gm](auto command)
            {
                UnitDescription ud1{command.unitId, static_cast<hp_type>(command.hp)};
                Point coord{static_cast<Point::coord_type>(command.x), static_cast<Point::coord_type>(command.y)};
                ArcherDescription descr{command.strength, 1, command.range, command.agility};
                gm.spawnUnit<LoggableUnit>(descr, ud1, coord);
            }).add<io::March>(
            [&gm](auto command)
            {
                Point coord{static_cast<Point::coord_type>(command.targetX), static_cast<Point::coord_type>(command.targetY)};
                gm.setMarchForUnit(command.unitId, coord);
            }).add<io::Wait>(
            [&gm](auto command)
            {
                gm.waitGameTicks(command.ticks);
            }).add<io::WaitPrintMap>(
            [&gm, &map](auto command)
            {   
                for(unsigned int i = 0; i < command.ticks; ++i)
                {
                    gm.waitOneGameTick();
                    std::cout << *map;
                }
            });
        
        parser.parse(file);
    } catch (std::exception & ex) {
        std::cerr << ex.what();
        return 1;
    }   
    return 0;
}