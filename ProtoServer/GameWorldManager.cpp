#include "stdafx.h"
#include "GameWorld.h"
#include "GameWorldManager.h"

GameWorldManager::GameWorldManager(asio::io_service & ioService) : _ioService(ioService)
{

}

shared_ptr<GameWorld> GameWorldManager::CreateWorld(int id)
{
	auto world = make_shared<GameWorld>(id, _ioService);

	lock_guard<mutex> lock(_lock);
	_worlds.insert(WorldMapType::value_type(id, world));

	return world;
}

shared_ptr<GameWorld> GameWorldManager::GetWorld(int id)
{
	lock_guard<mutex> lock(_lock);

	return _worlds.find(id)->second;
}
