// ProtoServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Network\TCPServer.h"
#include "Network\Session.h"
#include "Network\NullTerminateProtocolFilter.h"

int main()
{
	int port = 8080;

	LogHelper::GetInstance()->GetConsoleLogger()->info("Start server");
	LogHelper::GetInstance()->GetConsoleLogger()->info("Port : {}", port);
	LogHelper::GetInstance()->GetConsoleLogger()->info("IO Thread num : {}", 8);
	
	TCPServer server;
	server.Start(8, port, static_pointer_cast<IProtocolFilter>(make_shared<NullTerminateProtocolFilter>()));
	
	server.OnConnected = [](shared_ptr<ISession>& session)
	{
		LogHelper::GetInstance()->GetConsoleLogger()->info("connected");
	};

	server.OnClosed = [](shared_ptr<ISession>& session)
	{
		LogHelper::GetInstance()->GetConsoleLogger()->info("closed");
	};

	server.OnRecv = [] (shared_ptr<ISession>& session, asio::const_buffer& buf, int packet_len){
		const char* data = asio::buffer_cast<const char*>(buf);

		string s(data, packet_len);

		cout << "chat: " << s << endl;

		session->GetServer()->VisitSession([&s](auto visit_session)
		{
			visit_session->Send(s.data(), static_cast<int>(s.length()));
		});
	};

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

