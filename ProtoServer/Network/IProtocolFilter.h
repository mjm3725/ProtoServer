#pragma once

class IProtocolFilter
{
public:
	virtual int Parse(asio::const_buffer& buf) = 0;
};