#include "stdafx.h"
#include "TCPServer.h"
#include "SessionBase.h"



void TCPServer::Start(int thread_num, int port, shared_ptr<IProtocolFilter>& protocol_filter)
{
	protocol_filter_ = protocol_filter;
	acceptor_ = make_shared<tcp::acceptor>(io_service_, tcp::endpoint(tcp::v4(), port));

	DoAccept();

	for (int i = 0; i < thread_num; i++)
	{
		threads_.push_back(make_shared<thread>([this]
		{
			io_service_.run();
		}));
	}
}

void TCPServer::Stop()
{
	io_service_.stop();

	for (auto thread : threads_)
	{
		thread->join();
	}
}

void TCPServer::DoAccept()
{
	auto socket_ = make_shared<tcp::socket>(io_service_);

	acceptor_->async_accept(*socket_, [this, socket_](error_code ec) mutable
	{
		if (!ec)
		{
			auto session = CreateSession();

			session->Initialize(++current_handle_, socket_, this, [this](int64_t handle)
			{
				DeleteSession(handle);
			});
			
			AddSession(session);

			session->OnConnect();
			session->DoRecv();

			DoAccept();
		}
		else
		{
			cout << "Do accept error: " << ec.message();
		}
	});
}

void TCPServer::AddSession(shared_ptr<SessionBase>& session)
{
	lock_guard<mutex> lock(lock_);

	session_map_.insert(SessionMapValueType(session->GetHandle(), session));
}

void TCPServer::DeleteSession(int64_t handle)
{
	lock_guard<mutex> lock(lock_);

	session_map_.erase(handle);
}

void TCPServer::VisitSession(function<void(shared_ptr<SessionBase>&)> visitFunc)
{
	lock_guard<mutex> lock(lock_);

	for (auto& v : session_map_)
	{
		visitFunc(v.second);
	}
}
