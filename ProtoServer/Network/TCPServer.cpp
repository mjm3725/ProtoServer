#include "stdafx.h"
#include "TCPServer.h"
#include "SessionBase.h"

TCPServer::TCPServer(asio::io_service& ioService, int port, shared_ptr<SessionFactoryBase>& sessionFactory) :
	m_acceptor(ioService, tcp::endpoint(tcp::v4(), port)),
	m_socket(ioService),
	m_sessionFactory(sessionFactory)
{

}

void TCPServer::Start()
{
	DoAccept();
}


void TCPServer::DoAccept()
{
	m_acceptor.async_accept(m_socket, [this](error_code ec)
	{
		if (!ec)
		{
			auto session = m_sessionFactory->CreateSession(m_socket);

			session->OnConnect();
			session->DoRecv();

			DoAccept();
		}
		else
		{
			cout << "Do accept error: " << ec.message();
		}
	});
}