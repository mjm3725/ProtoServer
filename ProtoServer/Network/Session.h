#pragma once

using asio::ip::tcp;

class TCPServer;

class ISession
{
public:
	virtual int64_t GetHandle() = 0;
	virtual void Send(const void* data, int size) = 0;
	virtual TCPServer* GetServer() = 0;
	
};

class Session : public ISession, public enable_shared_from_this<Session>
{
public:
	enum
	{
		RECV_BUF_SIZE = 8192,
		SEND_BUF_SIZE = 8192,
	};

	void Initialize(
		int64_t handle, 
		shared_ptr<tcp::socket>& socket, 
		TCPServer* server);

	void DoRecv();
	void DoSend();

	void Send(const void* data, int size) override;
	int64_t GetHandle() override;
	TCPServer* GetServer() override;

	function<void(shared_ptr<Session>&)> OnClosed;
	function<void(shared_ptr<Session>&, asio::const_buffer& buf, int packet_len)> OnRecv;

private:
	shared_ptr<tcp::socket> socket_;
	asio::streambuf recv_buf_;
	asio::streambuf send_buf_;
	mutex send_buf_lock_;
	int64_t handle_;
	TCPServer* server_;
};
