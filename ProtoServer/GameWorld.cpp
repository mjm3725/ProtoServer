#include "stdafx.h"
#include "GameWorld.h"


GameWorld::GameWorld(int id, asio::io_service& ioService) : _id(id), _strand(ioService)
{
}

int GameWorld::GetId()
{
	return _id;
}

void GameWorld::Update()
{

}

void GameWorld::PushTask(function<void()>& task)
{
	_strand.post(task);
}


