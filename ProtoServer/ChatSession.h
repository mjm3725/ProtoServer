#pragma once

#include "Network/SessionBase.h"


class ChatSession :	public SessionBase
{
public:
	ChatSession(int64_t handle, tcp::socket& socket, SessionFactoryBase* sessionManager);
	~ChatSession();

	void OnConnect() override;
	void OnDisconnect() override;
	int OnRecv(asio::const_buffer& buf) override;
};

