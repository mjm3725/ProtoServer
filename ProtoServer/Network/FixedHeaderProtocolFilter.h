#pragma once


#include "IProtocolFilter.h"

class FixedHeaderProtocolFilter : public IProtocolFilter
{
public:
	enum
	{
		HEADER_SIZE = 6,
		HEADER_COMMAND_OFFSET = 0,
		HEADER_COMMAND_SIZE = 2,
		HEADER_BODY_LENGTH_OFFSET = 2,
		HEADER_BODY_LENGTH_SIZE = 4,
	};

	int Parse(asio::const_buffer& buf) override;
};

