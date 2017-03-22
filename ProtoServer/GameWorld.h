#pragma once

#include <concurrent_queue.h>
#include "TaskQueue.h"


class GameWorld
{
public:
	GameWorld(int id, asio::io_service& ioService);

	int GetId();

	void Update();

	void PushTask(function<void()>& task);

private:
	asio::io_service::strand _strand;
	chrono::time_point<chrono::system_clock> _lastTime;
	int _id;
};