// ProtoServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ChatServer.h"
#include "Network\NullTerminateProtocolFilter.h"

int main()
{
	int port = 8080;

	LogHelper::GetInstance()->GetConsoleLogger()->info("Start server");
	LogHelper::GetInstance()->GetConsoleLogger()->info("Port : {}", port);
	LogHelper::GetInstance()->GetConsoleLogger()->info("IO Thread num : {}", 8);
	
	ChatServer server;
	server.Start(8, port, static_pointer_cast<IProtocolFilter>(make_shared<NullTerminateProtocolFilter>()));

	while (true)
	{
		string s;

		cin >> s;

		if (s.compare("q") == 0)
		{
			break;
		}
	}

	server.Stop();

	return 0;
}

