#include "stdafx.h"
#include "GameObject.h"
#include "Network\Session.h"
#include "Packet\PacketHelper.h"



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

void GameObject::Send(int packetCommand, google::protobuf::Message& message)
{
	vector<char> buffer;
	PacketHelper::MakePacket(packetCommand, message, buffer);

	Send(buffer.data(), (int)buffer.size());
}

void GameObject::Send(const char* data, int size)
{
	auto session = _session.lock();

	if (!session)
	{
		return;
	}

	session->Send(data, (int)size);
}

void GameObject::GetGameObjectInfo(GameObjectInfo& gameObjectInfo)
{
	gameObjectInfo.set_handle(_handle);
	gameObjectInfo.set_name(_name);
	*gameObjectInfo.mutable_pos() = _pos;
}



