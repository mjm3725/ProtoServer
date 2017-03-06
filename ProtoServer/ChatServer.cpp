#include "stdafx.h"
#include "ChatServer.h"
#include "ChatSession.h"

shared_ptr<SessionBase> ChatServer::CreateSession()
{
	return static_pointer_cast<SessionBase>(make_shared<ChatSession>());
}
