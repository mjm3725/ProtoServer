#include "stdafx.h"
#include "TCPServer.h"
#include "ISessionFactory.h"
#include "SessionBase.h"
#include <iostream>

using namespace std;

TCPServer::TCPServer(asio::io_service& ioService, int port, ISessionFactory& sessionFactory) :
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
			// CreateSession후에 m_socket은 초기화됨(move)
			auto session = m_sessionFactory.CreateSession(m_socket);

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