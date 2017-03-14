#include "stdafx.h"
#include "Session.h"
#include "TCPServer.h"


void Session::Initialize(
	int64_t handle,
	shared_ptr<tcp::socket>& socket,
	TCPServer* server)
{
	handle_ = handle;
	socket_ = socket;
	server_ = server;
}

TCPServer* Session::GetServer()
{
	return server_;
}

int64_t Session::GetHandle()
{
	return handle_;
}


void Session::Send(const void* data, int size)
{
	lock_guard<mutex> lock(send_buf_lock_);

	size_t sendBufSize = send_buf_.size();

	if (sendBufSize + size > SEND_BUF_SIZE)
	{
		cout << "send buffer full" << endl;
		socket_->close();
		return;
	}

	bool isSend = false;

	if (sendBufSize == 0)
	{
		isSend = true;
	}
	
	auto buf = send_buf_.prepare(SEND_BUF_SIZE - sendBufSize);

	asio::buffer_copy(buf, asio::buffer(data, size));

	send_buf_.commit(size);

	if (isSend)
	{
		DoSend();
	}
}

void Session::DoSend()
{
	socket_->async_send(send_buf_.data(), [session = shared_from_this()](error_code ec, size_t bytesTransferred)
	{
		if (!ec)
		{
			lock_guard<mutex> lock(session->send_buf_lock_);

			session->send_buf_.consume(bytesTransferred);

			if (session->send_buf_.size() > 0)
			{
				session->DoSend();
			}
		}
		else
		{
			session->socket_->close();
		}
	});
}

void Session::DoRecv()
{
	// resize안되게 남은 input buffer크기만큼만 prepare함
	auto buf = recv_buf_.prepare(RECV_BUF_SIZE - recv_buf_.size());

	socket_->async_receive(buf, [session = shared_from_this()](error_code ec, size_t bytesTransferred) mutable
	{
		if (!ec)
		{
			session->recv_buf_.commit(bytesTransferred);

			int read_size = session->server_->GetProtocolFilter()->Parse(session->recv_buf_.data());

			if (read_size > 0)
			{
				session->OnRecv(session, session->recv_buf_.data(), read_size);

				session->recv_buf_.consume(read_size);
			}

			session->DoRecv();
		}
		else
		{
			session->OnClosed(session);
			session->socket_->close();
		}
	});
}