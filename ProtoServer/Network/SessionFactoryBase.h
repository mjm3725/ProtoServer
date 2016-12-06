#pragma once

class SessionBase;

using asio::ip::tcp;

class SessionFactoryBase
{
public:
	shared_ptr<SessionBase> CreateSession(tcp::socket& socket);
	void ReleaseSession(int64_t handle);

	void VisitSession(function<void(shared_ptr<SessionBase>&)> visitFunc);

	virtual shared_ptr<SessionBase> DoCreateSession(int64_t handle, tcp::socket& socket) = 0;

private:
	typedef unordered_map<int64_t, shared_ptr<SessionBase>>::value_type SessionMapValueType;

	atomic<int64_t> m_currentHandle;
	unordered_map<int64_t, shared_ptr<SessionBase>> m_sessionMap;
	mutex m_lock;
};
