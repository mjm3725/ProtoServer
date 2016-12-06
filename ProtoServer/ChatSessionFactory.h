#pragma once

#include "Network\ISessionFactory.h"

using namespace std;

class ChatSessionFactory : public ISessionFactory
{
public:
	shared_ptr<SessionBase> CreateSession(tcp::socket& socket) override;
};