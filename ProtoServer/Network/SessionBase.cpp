#include "stdafx.h"
#include "SessionBase.h"
#include "TCPServer.h"


void SessionBase::Initialize(int64_t handle, shared_ptr<tcp::socket>& socket, TCPServer* server, function<void(int64_t)> on_closed)
{
	handle_ = handle;
	socket_ = socket;
	server_ = server;
	on_closed_ = on_closed;
}

void SessionBase::OnConnect()
{

}

void SessionBase::OnDisconnect()
{

}

int64_t SessionBase::GetHandle()
{
	return handle_;
}

void SessionBase::OnRecv(asio::const_buffer& buf, int packet_len)
{
}

void SessionBase::Send(const void* data, int size)
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

void SessionBase::DoSend()
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

void SessionBase::DoRecv()
{
	// resize안되게 남은 input buffer크기만큼만 prepare함
	auto buf = recv_buf_.prepare(RECV_BUF_SIZE - recv_buf_.size());

	socket_->async_receive(buf, [session = shared_from_this()](error_code ec, size_t bytesTransferred)
	{
		if (!ec)
		{
			session->recv_buf_.commit(bytesTransferred);

			int read_size = session->server_->GetProtocolFilter()->Parse(session->recv_buf_.data());

			if (read_size > 0)
			{
				session->OnRecv(session->recv_buf_.data(), read_size);

				session->recv_buf_.consume(read_size);
			}

			session->DoRecv();
		}
		else
		{
			session->on_closed_(session->GetHandle());
			session->OnDisconnect();
			session->socket_->close();
		}
	});
}