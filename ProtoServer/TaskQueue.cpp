#include "stdafx.h"
#include "TaskQueue.h"

TaskQueue::TaskQueue(asio::io_service& ioService)
	: _ioService(ioService)
{

}

void TaskQueue::Push(function<void()>& task)
{
	int taskNum = _taskNum + 1;

	_taskQueue.push(task);

	if (taskNum == 1)
	{
		_ioService.post([taskQueue = shared_from_this()]() 
		{
			while (true)
			{
				int execNum = 0;
				function<void()> task;

				while (taskQueue->_taskQueue.try_pop(task))
				{
					task();
					execNum++;
				}

				int remainNum = taskQueue->_taskNum - execNum;

				if (remainNum == 0)
				{
					break;
				}
			}
		});
	}
}



