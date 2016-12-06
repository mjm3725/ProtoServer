#pragma once

#include "SessionFactoryBase.h"

using asio::ip::tcp;

class TCPServer
{
public:
	TCPServer(asio::io_service& ioService, int port, shared_ptr<SessionFactoryBase>& sessionFactory);

	void Start();

private:
	void DoAccept();

	tcp::acceptor m_acceptor;
	tcp::socket m_socket;
	shared_ptr<SessionFactoryBase> m_sessionFactory;
};

