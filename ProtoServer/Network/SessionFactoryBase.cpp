#include "stdafx.h"
#include "SessionFactoryBase.h"
#include "SessionBase.h"


shared_ptr<SessionBase> SessionFactoryBase::CreateSession(tcp::socket& socket)
{
	auto session = DoCreateSession(++m_currentHandle, socket);

	lock_guard<mutex> lock(m_lock);
	
	m_sessionMap.insert(SessionMapValueType(session->GetHandle(), session));

	return session;
}

void SessionFactoryBase::ReleaseSession(int64_t handle)
{
	lock_guard<mutex> lock(m_lock);

	m_sessionMap.erase(handle);
}

void SessionFactoryBase::VisitSession(function<void(shared_ptr<SessionBase>&)> visitFunc)
{
	lock_guard<mutex> lock(m_lock);

	for (auto& v : m_sessionMap)
	{
		visitFunc(v.second);
	}
}
