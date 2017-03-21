#pragma once

#include <concurrent_queue.h>

class GameWorld
{
public:
	void Update();

	void PushTask(function<void()>& task);

private:
	Concurrency::concurrent_queue<function<void()>> _taskQueue;

	chrono::time_point<chrono::system_clock> _lastTime;
};