// ProtoServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

