#pragma once

using asio::ip::tcp;

class TCPServer;

class ISessionState
{
	
};

class ISession
{
public:
	virtual int64_t GetHandle() = 0;
	virtual void Send(const void* data, int size) = 0;
	virtual TCPServer& GetServer() = 0;
	virtual void SetSessionState(shared_ptr<ISessionState>& sessionState) = 0;
	virtual ISessionState& GetSessionState() = 0;
};

class Session : public ISession, public enable_shared_from_this<Session>
{
public:
	enum
	{
		RECV_BUF_SIZE = 8192,
		SEND_BUF_SIZE = 8192,
	};

	Session(int64_t handle, tcp::socket& socket, TCPServer& server, const function<void(shared_ptr<Session>&, error_code&)>& onClosed);

	void AsyncRecv();
	
	void Send(const void* data, int size) override;
	int64_t GetHandle() override;
	TCPServer& GetServer() override;
	void SetSessionState(shared_ptr<ISessionState>& sessionState) override;
	ISessionState& GetSessionState() override;
	
private:
	void AsyncSend();

	function<void(shared_ptr<Session>&, error_code&)> _onClosed;

	tcp::socket _socket;
	asio::streambuf _recvBuf;
	asio::streambuf _sendBuf;
	mutex _sendBufLock;
	int64_t _handle;
	TCPServer& _server;
	shared_ptr<ISessionState> _sessionState;
};
