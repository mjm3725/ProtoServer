#include "stdafx.h"
#include "Session.h"
#include "TCPServer.h"


void Session::Initialize(int64_t handle, shared_ptr<tcp::socket>& socket, TCPServer* server)
{
	_handle = handle;
	_socket = socket;
	_server = server;
}

TCPServer* Session::GetServer()
{
	return _server;
}

int64_t Session::GetHandle()
{
	return _handle;
}


void Session::Send(const void* data, int size)
{
	lock_guard<mutex> lock(_sendBufLock);

	size_t sendBufSize = _sendBuf.size();

	if (sendBufSize + size > SEND_BUF_SIZE)
	{
		cout << "send buffer full" << endl;
		_socket->close();
		return;
	}

	bool isSend = false;

	if (sendBufSize == 0)
	{
		isSend = true;
	}
	
	auto buf = _sendBuf.prepare(SEND_BUF_SIZE - sendBufSize);

	asio::buffer_copy(buf, asio::buffer(data, size));

	_sendBuf.commit(size);

	if (isSend)
	{
		DoSend();
	}
}

void Session::DoSend()
{
	_socket->async_send(_sendBuf.data(), [session = shared_from_this()](error_code ec, size_t bytesTransferred)
	{
		if (!ec)
		{
			lock_guard<mutex> lock(session->_sendBufLock);

			session->_sendBuf.consume(bytesTransferred);

			if (session->_sendBuf.size() > 0)
			{
				session->DoSend();
			}
		}
		else
		{
			session->_socket->close();
		}
	});
}

void Session::DoRecv()
{
	// resize안되게 남은 input buffer크기만큼만 prepare함
	auto buf = _recvBuf.prepare(RECV_BUF_SIZE - _recvBuf.size());

	_socket->async_receive(buf, [session = shared_from_this()](error_code ec, size_t bytesTransferred) mutable
	{
		if (!ec)
		{
			session->_recvBuf.commit(bytesTransferred);

			int read_size = session->_server->GetProtocolFilter()->Parse(session->_recvBuf.data());

			if (read_size > 0)
			{
				session->OnRecv(session, session->_recvBuf.data(), read_size);

				session->_recvBuf.consume(read_size);
			}

			session->DoRecv();
		}
		else
		{
			session->OnClosed(session, ec);
			session->_socket->close();
		}
	});
}