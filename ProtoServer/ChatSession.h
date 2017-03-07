#pragma once

#include "Network/SessionBase.h"


class ChatSession :	public SessionBase
{
public:
	void OnConnect() override;
	void OnDisconnect() override;
	void OnRecv(asio::const_buffer& buf, int packet_len) override;
};

