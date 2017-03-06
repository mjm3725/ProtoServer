#pragma once

#include "Network/SessionBase.h"


class ChatSession :	public SessionBase
{
public:
	void OnConnect() override;
	void OnDisconnect() override;
	int OnRecv(asio::const_buffer& buf) override;
};

