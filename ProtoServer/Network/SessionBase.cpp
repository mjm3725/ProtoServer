#include "stdafx.h"
#include "SessionBase.h"
#include "SessionFactoryBase.h"

SessionBase::SessionBase(int64_t handle, tcp::socket& socket, SessionFactoryBase* sessionFactory)
	: m_handle(handle),
	m_socket(std::move(socket)),
	m_sessionFactory(sessionFactory)
{

}

SessionBase::~SessionBase()
{

}

void SessionBase::OnConnect()
{

}

void SessionBase::OnDisconnect()
{

}

int64_t SessionBase::GetHandle()
{
	return m_handle;
}

int SessionBase::OnRecv(asio::const_buffer& buf)
{
	return 0;
}

void SessionBase::Send(const void* data, int size)
{
	lock_guard<mutex> lock(m_lock);

	size_t sendBufSize = m_sendBuf.size();

	if (sendBufSize + size > SEND_BUF_SIZE)
	{
		cout << "send buffer full" << endl;
		m_socket.close();
		return;
	}

	bool isSend = false;

	if (sendBufSize == 0)
	{
		isSend = true;
	}
	
	auto buf = m_sendBuf.prepare(SEND_BUF_SIZE - sendBufSize);

	asio::buffer_copy(buf, asio::buffer(data, size));

	m_sendBuf.commit(size);

	if (isSend)
	{
		DoSend();
	}
}

void SessionBase::DoSend()
{
	m_socket.async_send(m_sendBuf.data(), [session = shared_from_this()](error_code ec, size_t bytesTransferred)
	{
		cout << "sendbyte : " << bytesTransferred << endl;

		if (!ec)
		{
			lock_guard<mutex> lock(session->m_lock);

			session->m_sendBuf.consume(bytesTransferred);

			if (session->m_sendBuf.size() > 0)
			{
				session->DoSend();
			}
		}
		else
		{
			cout << "send error : " << ec.message() << endl;
			session->m_socket.close();
		}
	});
}

void SessionBase::DoRecv()
{
	// resize안되게 남은 input buffer크기만큼만 prepare함
	auto buf = m_recvBuf.prepare(RECV_BUF_SIZE - m_recvBuf.size());

	m_socket.async_receive(buf, [session = shared_from_this()](error_code ec, size_t bytesTransferred)
	{
		cout << "recvbyte : " << bytesTransferred << endl;

		if (!ec)
		{
			session->m_recvBuf.commit(bytesTransferred);

			size_t readSize = session->OnRecv(session->m_recvBuf.data());
			
			if (readSize > 0)
			{
				session->m_recvBuf.consume(readSize);
			}

			session->DoRecv();
		}
		else
		{
			cout << "recv error : " << ec.message() << endl;
			
			session->m_sessionFactory->ReleaseSession(session->GetHandle());
			session->OnDisconnect();
			session->m_socket.close();
		}
	});
}