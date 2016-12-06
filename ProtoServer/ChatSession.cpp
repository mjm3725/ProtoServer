#include "stdafx.h"
#include "ChatSession.h"
#include <iostream>
#include <array>

using namespace std;

ChatSession::ChatSession(tcp::socket& socket) : SessionBase(socket)
{
}


ChatSession::~ChatSession()
{
}

void ChatSession::OnConnect()
{
	cout << "connected : " << m_socket.remote_endpoint().address().to_string() << endl;
}

void ChatSession::OnDisconnect()
{
	cout << "disconnected : " << m_socket.remote_endpoint().address().to_string() << endl;
}

int ChatSession::OnRecv(asio::const_buffer& buf)
{
	const char* data = asio::buffer_cast<const char*>(buf);
	size_t size = asio::buffer_size(buf);

	for (int i = 0; i < size; i++)
	{
		if (data[i] == '\n')
		{
			string s(data, i + 1);

			cout << "chat: " << s;

			Send(data, i + 1);

			return i + 1;
		}
	}

	return 0;
}