#pragma once

using asio::ip::tcp;

class SessionFactoryBase;

class SessionBase : public enable_shared_from_this<SessionBase>
{
public:
	enum
	{
		RECV_BUF_SIZE = 8192,
		SEND_BUF_SIZE = 8192,
	};

	SessionBase(int64_t handle, tcp::socket& socket, SessionFactoryBase* sessionFactory);
	virtual ~SessionBase();

	virtual void OnConnect();
	virtual void OnDisconnect();
	virtual int OnRecv(asio::const_buffer& buf);

	void DoRecv();
	void DoSend();

	void Send(const void* data, int size);

	int64_t GetHandle();

protected:
	tcp::socket m_socket;
	asio::streambuf m_recvBuf;
	asio::streambuf m_sendBuf;
	mutex m_lock;
	int64_t m_handle;
	SessionFactoryBase* m_sessionFactory;
};
