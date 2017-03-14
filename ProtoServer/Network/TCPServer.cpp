#include "stdafx.h"
#include "TCPServer.h"
#include "Session.h"



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
			auto new_session = make_shared<Session>();

			new_session->Initialize(++current_handle_, socket_, this);
				
			new_session->OnClosed = [this](shared_ptr<Session>& session)
			{
				DeleteSession(session->GetHandle());
				OnClosed(static_pointer_cast<ISession>(session));
			};
				
			new_session->OnRecv = [this](shared_ptr<Session>& session, asio::const_buffer& buf, int packet_len)
			{
				OnRecv(static_pointer_cast<ISession>(session), buf, packet_len);
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

void TCPServer::AddSession(shared_ptr<Session>& session)
{
	lock_guard<mutex> lock(lock_);

	session_map_.insert(SessionMapValueType(session->GetHandle(), session));
}

void TCPServer::DeleteSession(int64_t handle)
{
	lock_guard<mutex> lock(lock_);

	session_map_.erase(handle);
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
	lock_.lock();
	sessions.reserve(session_map_.size());
	lock_.unlock();
		
	for(auto& v : session_map_)
	{
		sessions.push_back(static_pointer_cast<ISession>(v.second));
	}
}

IProtocolFilter* TCPServer::GetProtocolFilter()
{
	return protocol_filter_.get();
}