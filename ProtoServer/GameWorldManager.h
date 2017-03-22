#pragma once

#include <unordered_map>

class GameWorld;

class GameWorldManager
{
public:
	GameWorldManager(asio::io_service& ioService);
	
	shared_ptr<GameWorld> CreateWorld(int id);
	shared_ptr<GameWorld> GetWorld(int id);

private:
	typedef std::unordered_map<int, shared_ptr<GameWorld>> WorldMapType;

	WorldMapType _worlds;
	asio::io_service& _ioService;
	mutex _lock;
};
