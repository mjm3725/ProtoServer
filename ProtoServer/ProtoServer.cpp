// ProtoServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Network/TCPServer.h"
#include "ChatSessionFactory.h"

int main()
{
	int port = 10000;

	LogHelper::GetInstance()->GetConsoleLogger()->info("Start server");
	LogHelper::GetInstance()->GetConsoleLogger()->info("Port : {}", port);
	
	asio::io_service ioService;

	TCPServer server(ioService, port, static_pointer_cast<SessionFactoryBase>(make_shared<ChatSessionFactory>()));
	server.Start();

	ioService.run();

	while (true)
	{
		Sleep(100);
	}

	return 0;
}

