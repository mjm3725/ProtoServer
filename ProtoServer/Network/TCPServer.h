#pragma once

#include "IProtocolFilter.h"


using asio::ip::tcp;

class ISession;
class Session;


class TCPServer
{
public:
	TCPServer(asio::io_service& ioService);

	void Start(int threadNum, int port, shared_ptr<IProtocolFilter>& protocolFilter);
	void Stop();

	void VisitSession(const function<void(shared_ptr<ISession>&)>& visitFunc);

	IProtocolFilter* GetProtocolFilter();

	function<void(shared_ptr<ISession>&)> OnConnected;
	function<void(shared_ptr<ISession>&, error_code& errorCode)> OnClosed;
	function<void(shared_ptr<ISession>&, asio::const_buffer& buf, int packetLen)> OnRecv;

	asio::io_service& GetIOService();

private:
	void AddSession(shared_ptr<Session>& session);
	void DeleteSession(int64_t handle);
	void CopySession(vector<shared_ptr<ISession>>& sessions);

	void DoAccept();

	tcp::acceptor _acceptor;
	asio::io_service& _ioService;
	tcp::socket _socket;

	shared_ptr<IProtocolFilter> _protocolFilter;
	vector<thread> _threads;
	atomic<int64_t> _currentHandle;

	typedef unordered_map<int64_t, shared_ptr<Session>>::value_type SessionMapValueType;
	unordered_map<int64_t, shared_ptr<Session>> _sessionMap;

	mutex _lock;
};

