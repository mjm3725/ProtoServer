#pragma once

#include "asio.hpp"
#include <mutex>

using asio::ip::tcp;
using namespace std;


class SessionBase : public enable_shared_from_this<SessionBase>
{
public:
	enum
	{
		RECV_BUF_SIZE = 8192,
		SEND_BUF_SIZE = 8192,
	};

	SessionBase(tcp::socket& socket);

	virtual ~SessionBase();

	virtual void OnConnect();
	virtual void OnDisconnect();
	virtual int OnRecv(asio::const_buffer& buf);

	void DoRecv();
	void DoSend();

	void Send(const void* data, int size);

protected:
	tcp::socket m_socket;
	asio::streambuf m_recvBuf;
	asio::streambuf m_sendBuf;

	mutex m_lock;
};
