#pragma once

#include "IProtocolFilter.h"


using asio::ip::tcp;

class SessionBase;


class TCPServer
{
public:
	void Start(int thread_num, int port, shared_ptr<IProtocolFilter>& protocol_filter);
	void Stop();

	void VisitSession(function<void(shared_ptr<SessionBase>&)> visitFunc);

	virtual shared_ptr<SessionBase> CreateSession() = 0;

private:
	void AddSession(shared_ptr<SessionBase>& session);
	void DeleteSession(int64_t handle);

	void DoAccept();

	shared_ptr<tcp::acceptor> acceptor_;
	shared_ptr<IProtocolFilter> protocol_filter_;
	asio::io_service io_service_;
	vector<shared_ptr<thread>> threads_;

	atomic<int64_t> current_handle_;

	typedef unordered_map<int64_t, shared_ptr<SessionBase>>::value_type SessionMapValueType;
	unordered_map<int64_t, shared_ptr<SessionBase>> session_map_;

	mutex lock_;
};

