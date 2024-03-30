#include "Units/Unit.hpp"
#include <IO/System/CommandParser.hpp>
#include <IO/System/PrintDebug.hpp>
#include <iostream>
#include <fstream>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/Wait.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/EventLogs/MapCreated.hpp>
#include <IO/EventLogs/UnitSpawned.hpp>
#include <IO/EventLogs/MarchStarted.hpp>
#include <IO/EventLogs/MarchEnded.hpp>
#include <IO/EventLogs/UnitMoved.hpp>
#include <IO/EventLogs/UnitDied.hpp>
#include <IO/EventLogs/UnitAttacked.hpp>

#include "Units/UnitBuilder.hpp"
#include "Map/Map.hpp"

#include <vector>

int main(int argc, char** argv)
{
	using namespace sw;

	units::UnitBuilder<units::UnitClass::WAR, units::Unit> warBuilder;
	warBuilder.add_action_by_priority(0, 
	[](units::IUnit& uRef, bool f){
		std::cout << "zero war action" << std::endl;
		return false;
	});
	warBuilder.add_action_by_priority(1, 
	[](units::IUnit& uRef, bool f){
		std::cout << "first war action" << std::endl;
		return false;
	});

	units::UnitBuilder<units::UnitClass::ARCH, units::Unit> archBuilder;
	archBuilder.add_action_by_priority(0, 
	[](units::IUnit& uRef, bool f){
		std::cout << "zero arch action" << std::endl;
		return false;
	});
	archBuilder.add_action_by_priority(1, 
	[](units::IUnit& uRef, bool f){
		std::cout << "first arch action" << std::endl;
		return false;
	});

	std::vector<std::unique_ptr<units::IUnit>> unitsOnMap;
	for(int i = 0; i < 10; ++i)
	{
		unitsOnMap.emplace_back(warBuilder.create_unit(i));
		unitsOnMap.emplace_back(archBuilder.create_unit(i + 10));
	}


    // Создаем объект карты размером 5x5
    map::Map<units::IUnit> map(5, 5);
	std::cout << map;
	
	map.addUnit(0, 2, unitsOnMap[0]->clone());
	map.addUnit(4, 2, unitsOnMap[1]->clone());
	std::cout << map;


	// for(auto& unit : unitsOnMap)
	// 	unit->march_process();
	
	// if (argc != 2) {
	// 	throw std::runtime_error("Error: No file specified in command line argument");
	// }

	// std::ifstream file(argv[1]);
	// if (!file) {
	// 	throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	// }

	// // Code for example...

	// std::cout << "Commands:\n";
	// io::CommandParser parser;
	// parser.add<io::CreateMap>(
	// 	[](auto command)
	// 	{
	// 		printDebug(std::cout, command);
	// 	}).add<io::SpawnWarrior>(
	// 	[](auto command)
	// 	{
	// 		printDebug(std::cout, command);
	// 	}).add<io::SpawnArcher>(
	// 	[](auto command)
	// 	{
	// 		printDebug(std::cout, command);
	// 	}).add<io::March>(
	// 	[](auto command)
	// 	{
	// 		printDebug(std::cout, command);
	// 	}).add<io::Wait>(
	// 	[](auto command)
	// 	{
	// 		printDebug(std::cout, command);
	// 	});

	// parser.parse(file);

	// std::cout << "\n\nEvents:\n";

	// EventLog eventLog;
	// eventLog.listen<io::MapCreated>([](auto& event){ printDebug(std::cout, event); });
	// eventLog.listen<io::UnitSpawned>([](auto& event){ printDebug(std::cout, event); });
	// eventLog.listen<io::MarchStarted>([](auto& event){ printDebug(std::cout, event); });
	// eventLog.listen<io::UnitMoved>([](auto& event){ printDebug(std::cout, event); });
	// eventLog.listen<io::MarchEnded>([](auto& event){ printDebug(std::cout, event); });
	// eventLog.listen<io::UnitAttacked>([](auto& event){ printDebug(std::cout, event); });
	// eventLog.listen<io::UnitDied>([](auto& event){ printDebug(std::cout, event); });

	// eventLog.log(io::MapCreated{ 10, 10 });
	// eventLog.log(io::UnitSpawned{ 1, "Archer", 5, 3 });
	// eventLog.log(io::UnitSpawned{ 2, "Warrior", 5, 3 });
	// eventLog.log(io::MarchStarted{ 1, 5, 3, 7, 9 });
	// eventLog.log(io::UnitMoved{ 1, 6, 4 });
	// eventLog.log(io::UnitAttacked{ 1, 2, 5, 0 });
	// eventLog.log(io::MarchEnded{ 1, 7, 9 });
	// eventLog.log(io::UnitDied{ 1 });

	return 0;
}