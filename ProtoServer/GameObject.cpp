#include "stdafx.h"
#include "GameObject.h"
#include "Network\Session.h"

GameObject::GameObject(__int64 handle, string & name) : _handle(handle), _name(name)
{
}

void GameObject::Update(float elapsedTime)
{

}

__int64 GameObject::GetHandle()
{
	return _handle;
}

void GameObject::SetSession(shared_ptr<ISession>& session)
{
	_session = session;
}

void GameObject::Send(const char * data, int size)
{
	auto session = _session.lock();

	if (!session)
	{
		return;
	}

	session->Send(data, size);
}



