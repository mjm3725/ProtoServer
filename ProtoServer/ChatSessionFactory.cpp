#include "stdafx.h"
#include "ChatSessionFactory.h"
#include "ChatSession.h"


shared_ptr<SessionBase> ChatSessionFactory::DoCreateSession(int64_t handle, tcp::socket& socket)
{
	return make_shared<ChatSession>(handle, socket, this);
}