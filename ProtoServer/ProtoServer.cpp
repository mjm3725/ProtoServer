// ProtoServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Network\TCPServer.h"
#include "Network\Session.h"
#include "Network\FixedHeaderProtocolFilter.h"
#include "GameWorld.h"
#include "GameWorldManager.h"
#include "PlayerState.h"
#include "Packet\PacketDispatcher.h"


int main()
{
	int port = 8080;

	LogHelper::GetInstance()->GetConsoleLogger()->set_level(spdlog::level::debug);
	LogHelper::GetInstance()->GetConsoleLogger()->info("Start server");
	LogHelper::GetInstance()->GetConsoleLogger()->info("Port : {}", port);
	LogHelper::GetInstance()->GetConsoleLogger()->info("IO Thread num : {}", 8);
	
	asio::io_service ioService;

	TCPServer server(ioService, port, static_pointer_cast<IProtocolFilter>(make_shared<FixedHeaderProtocolFilter>()));
	
	GameWorldManager::CreateInstance(ioService);
	GameWorldManager::GetInstance()->CreateWorld(1);

	PacketDispatcher dispatcher;
	dispatcher.Initialize();


	server.OnConnected = [](shared_ptr<ISession>& session)
	{
		session->SetSessionState(static_pointer_cast<ISessionState>(make_shared<PlayerState>()));

		LogHelper::GetInstance()->GetConsoleLogger()->debug("connected");
	};

	server.OnClosed = [](shared_ptr<ISession>& session, error_code& errorCode)
	{
		auto playerState = (PlayerState*)session->GetSessionState();

		playerState->Dispose();
		GameWorldManager::GetInstance()->GetWorld(1)->LeaveGameObject(playerState->GameObject);

		LogHelper::GetInstance()->GetConsoleLogger()->debug("closed");
	};

	server.OnRecv = [&dispatcher](shared_ptr<ISession>& session, asio::const_buffer& buf, int packetLen)
	{
		dispatcher.Dispatch(session, buf, packetLen);
	};
	
	server.Start(8);
	
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

