#pragma once


class ISession;

class GameObject : public enable_shared_from_this<GameObject>
{
public: 
	GameObject(__int64 handle, string& name);

	void Update(float elapsedTime);

	__int64 GetHandle();

	void SetSession(shared_ptr<ISession>& session);
	void Send(const char* data, int size);

private:
	__int64 _handle;
	string _name;

	weak_ptr<ISession> _session;
};
