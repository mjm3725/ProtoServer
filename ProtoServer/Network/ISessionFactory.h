#pragma once


#include "asio.hpp"

class SessionBase;

using asio::ip::tcp;

class ISessionFactory
{
public:
	virtual std::shared_ptr<SessionBase> CreateSession(tcp::socket& socket) = 0;
};
