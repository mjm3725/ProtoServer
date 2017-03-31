#include "stdafx.h"
#include "GameWorld.h"
#include "GameWorldManager.h"

unique_ptr<GameWorldManager> GameWorldManager::_instance;

void GameWorldManager::CreateInstance(asio::io_service & ioService)
{
	_instance = make_unique<GameWorldManager>(ioService);
}

GameWorldManager* GameWorldManager::GetInstance()
{
	return _instance.get();
}

GameWorldManager::GameWorldManager(asio::io_service & ioService) : _ioService(ioService)
{

}

shared_ptr<GameWorld>& GameWorldManager::CreateWorld(int id)
{
	auto world = make_shared<GameWorld>(id, _ioService);

	lock_guard<mutex> lock(_lock);
	auto ret = _worlds.insert(WorldMapType::value_type(id, world));
	
	return ret.first->second;
}

shared_ptr<GameWorld>& GameWorldManager::GetWorld(int id)
{
	lock_guard<mutex> lock(_lock);

	return _worlds.find(id)->second;
}
