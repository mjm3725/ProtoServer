#pragma once

#include "Network\SessionFactoryBase.h"

class ChatSessionFactory : public SessionFactoryBase
{
public:
	shared_ptr<SessionBase> DoCreateSession(int64_t handle, tcp::socket& socket) override;
};