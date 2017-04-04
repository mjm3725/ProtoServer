#pragma once

#include "Protocol\Protocol.pb.h"

class ISession;

class GameObject : public enable_shared_from_this<GameObject>
{
public: 
	GameObject(__int64 handle, string& name);

	void Update(float elapsedTime);

	__int64 GetHandle();

	void SetSession(shared_ptr<ISession>& session);
	void Send(int packetCommand, google::protobuf::Message& message);
	void Send(const char* data, int size);

	void GetGameObjectInfo(GameObjectInfo& gameObjectInfo);

private:
	__int64 _handle;
	string _name;
	PkVector3 _pos;
	weak_ptr<ISession> _session;
};
