#pragma once

#include <concurrent_queue.h>



class TaskQueue : enable_shared_from_this<TaskQueue>
{
public:
	TaskQueue(asio::io_service& ioService);

	void Push(function<void()>& task);

private:
	Concurrency::concurrent_queue<function<void()>> _taskQueue;
	atomic_int32_t _taskNum;
	asio::io_service& _ioService;
};
