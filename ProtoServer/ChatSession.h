#pragma once

#include "Network/SessionBase.h"

class ChatSession :	public SessionBase
{
public:
	ChatSession(tcp::socket& socket);
	~ChatSession();

	void OnConnect() override;
	void OnDisconnect() override;
	int OnRecv(asio::const_buffer& buf) override;
};

