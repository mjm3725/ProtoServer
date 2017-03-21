#include "stdafx.h"
#include "GameWorld.h"

void GameWorld::Update()
{

}

void GameWorld::PushTask(function<void()>& task)
{
	_taskQueue.push(task);
}


