#pragma once

#include "asio.hpp"

using asio::ip::tcp;

class ISessionFactory;

class TCPServer
{
public:
	TCPServer(asio::io_service& ioService, int port, ISessionFactory& sessionFactory);

	void Start();

private:
	void DoAccept();

	tcp::acceptor m_acceptor;
	tcp::socket m_socket;
	ISessionFactory& m_sessionFactory;
};

