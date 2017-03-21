#pragma once


class GameProcessor
{
public:
	void Start(int threadNum);
	void Stop();

	
private:
	vector<shared_ptr<thread>> _threads;
	asio::io_service _ioService;
};
