#pragma once
#include "IProtocolFilter.h"

class NullTerminateProtocolFilter :	public IProtocolFilter
{
public:
	int Parse(asio::const_buffer& buf) override;
};

