#include "stdafx.h"
#include "TCPServer.h"
#include "Session.h"



void TCPServer::Start(int threadNum, int port, shared_ptr<IProtocolFilter>& protocolFilter)
{
	_protocolFilter = protocolFilter;
	_acceptor = make_shared<tcp::acceptor>(_ioService, tcp::endpoint(tcp::v4(), port));

	DoAccept();

	_threads.reserve(threadNum);

	for (int i = 0; i < threadNum; i++)
	{
		_threads.push_back(make_shared<thread>([this]
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
		thread->join();
	}
}

void TCPServer::DoAccept()
{
	auto _socket = make_shared<tcp::socket>(_ioService);

	_acceptor->async_accept(*_socket, [this, _socket](error_code ec) mutable
	{
		if (!ec)
		{
			auto new_session = make_shared<Session>();

			new_session->Initialize(++_currentHandle, _socket, this);
				
			new_session->OnClosed = [this](shared_ptr<Session>& session, error_code& errorCode)
			{
				DeleteSession(session->GetHandle());
				OnClosed(static_pointer_cast<ISession>(session), errorCode);
			};
				
			new_session->OnRecv = [this](shared_ptr<Session>& session, asio::const_buffer& buf, int packetLen)
			{
				OnRecv(static_pointer_cast<ISession>(session), buf, packetLen);
			};

			AddSession(new_session);

			OnConnected(static_pointer_cast<ISession>(new_session));

			new_session->DoRecv();

			DoAccept();
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

void TCPServer::VisitSession(function<void(shared_ptr<ISession>&)> visitFunc)
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