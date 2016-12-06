// ProtoServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Network/TCPServer.h"
#include "ChatSessionFactory.h"

int main()
{
	asio::io_service ioService;
	ChatSessionFactory chatSessionFactory;

	TCPServer server(ioService, 10000, chatSessionFactory);
	server.Start();

	ioService.run();

	while (true)
	{
		Sleep(100);
	}

    return 0;
}

