#pragma once


#include "IProtocolFilter.h"

class FixedHeaderProtocolFilter : public IProtocolFilter
{
public:
	enum
	{
		HeaderSize = 6,
		CommandOffset = 0,
		CommandSize = 2,
		BodyLengthOffset = 2,
		BodyLengthSize = 4,
	};

	int Parse(asio::const_buffer& buf) override;
};

