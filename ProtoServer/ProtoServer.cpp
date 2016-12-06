// ProtoServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

