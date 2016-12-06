#include "stdafx.h"
#include "ChatSessionFactory.h"
#include "ChatSession.h"


shared_ptr<SessionBase> ChatSessionFactory::CreateSession(tcp::socket& socket)
{
	return make_shared<ChatSession>(socket);
}