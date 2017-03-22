#include "stdafx.h"
#include "TCPServer.h"
#include "Session.h"


TCPServer::TCPServer(asio::io_service& ioService)
	: _ioService(ioService),
	_socket(ioService),
	_acceptor(ioService)
{

}

void TCPServer::Start(int threadNum, int port, shared_ptr<IProtocolFilter>& protocolFilter)
{
	_protocolFilter = protocolFilter;
	_acceptor.bind(tcp::endpoint(tcp::v4(), port));
	
	AsyncAccept();

	_threads.reserve(threadNum);

	for (int i = 0; i < threadNum; i++)
	{
		_threads.push_back(thread([this]
		{
			_ioService.run();
		}));
	}
}

void TCPServer::Stop()
{
	_ioService.stop();

	for (auto& thread : _threads)
	{
		thread.join();
	}
}

void TCPServer::AsyncAccept()
{
	_acceptor.async_accept(_socket, [this](error_code ec) mutable
	{
		if (!ec)
		{
			auto new_session = make_shared<Session>(++_currentHandle, _socket, *this, [this](shared_ptr<Session>& session, error_code& errorCode)
			{
				DeleteSession(session->GetHandle());
				OnClosed(static_pointer_cast<ISession>(session), errorCode);
			});

			AddSession(new_session);

			OnConnected(static_pointer_cast<ISession>(new_session));

			new_session->AsyncRecv();

			AsyncAccept();
		}
		else
		{
			cout << "Do accept error: " << ec.message();
		}
	});
}

asio::io_service & TCPServer::GetIOService()
{
	return _ioService;
}

void TCPServer::AddSession(shared_ptr<Session>& session)
{
	lock_guard<mutex> lock(_lock);

	_sessionMap.insert(SessionMapValueType(session->GetHandle(), session));
}

void TCPServer::DeleteSession(int64_t handle)
{
	lock_guard<mutex> lock(_lock);

	_sessionMap.erase(handle);
}

void TCPServer::VisitSession(const function<void(shared_ptr<ISession>&)>& visitFunc)
{
	vector<shared_ptr<ISession>> sessions;

	CopySession(sessions);
	 
	for (auto& v : sessions)
	{
		visitFunc(v);
	}
}

void TCPServer::CopySession(vector<shared_ptr<ISession>>& sessions)
{
	_lock.lock();
	sessions.reserve(_sessionMap.size());
	_lock.unlock();
		
	for(auto& v : _sessionMap)
	{
		sessions.push_back(static_pointer_cast<ISession>(v.second));
	}
}

IProtocolFilter* TCPServer::GetProtocolFilter()
{
	return _protocolFilter.get();
}