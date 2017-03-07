#include "stdafx.h"
#include "ChatSession.h"
#include "Network/TCPServer.h"

void ChatSession::OnConnect()
{
	cout << "connected : " << socket_->remote_endpoint().address().to_string() << endl;
}

void ChatSession::OnDisconnect()
{
	cout << "disconnected : " << socket_->remote_endpoint().address().to_string() << endl;
}

void ChatSession::OnRecv(asio::const_buffer& buf, int packet_len)
{
	const char* data = asio::buffer_cast<const char*>(buf);

	string s(data, packet_len);

	cout << "chat: " << s << endl;

	server_->VisitSession([&s](auto session)
	{
		session->Send(s.data(), static_cast<int>(s.length()));
	});
}