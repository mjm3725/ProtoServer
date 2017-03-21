#include "stdafx.h"
#include "GameProcessor.h"

void GameProcessor::Start(int threadNum)
{
	_threads.reserve(threadNum);

	for (int i = 0; i < threadNum; i++)
	{
		_threads.push_back(make_shared<thread>([this]() 
		{
			_ioService.run();
		}));
	}
}

void GameProcessor::Stop()
{
	_ioService.stop();

	for (auto& thread : _threads)
	{
		thread->join();
	}
}