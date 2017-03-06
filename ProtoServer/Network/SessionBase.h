#pragma once

using asio::ip::tcp;

class TCPServer;

class SessionBase : public enable_shared_from_this<SessionBase>
{
public:
	enum
	{
		RECV_BUF_SIZE = 8192,
		SEND_BUF_SIZE = 8192,
	};

	void Initialize(int64_t handle, shared_ptr<tcp::socket>& socket, TCPServer* server, function<void(int64_t)> on_closed);

	virtual void OnConnect();
	virtual void OnDisconnect();
	virtual int OnRecv(asio::const_buffer& buf);

	void DoRecv();
	void DoSend();

	void Send(const void* data, int size);

	int64_t GetHandle();

protected:
	shared_ptr<tcp::socket> socket_;
	asio::streambuf recv_buf_;
	asio::streambuf send_buf_;
	mutex send_buf_lock_;
	int64_t handle_;
	TCPServer* server_;
	function<void(int64_t)> on_closed_;
};
