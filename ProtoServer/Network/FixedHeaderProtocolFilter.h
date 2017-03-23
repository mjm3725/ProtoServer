#pragma once


#include "IProtocolFilter.h"

class FixedHeaderProtocolFilter : public IProtocolFilter
{
public:
	enum
	{
		HeaderSize = 6,
		CommandSize = 2,
	};

	int Parse(asio::const_buffer& buf) override;
};

