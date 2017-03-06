#pragma once

#include "Network\TCPServer.h"

class ChatServer : public TCPServer
{
public:
	shared_ptr<SessionBase> CreateSession() override;
};