#include "stdafx.h"
#include "FixedHeaderProtocolFilter.h"


int FixedHeaderProtocolFilter::Parse(asio::const_buffer& buf)
{
	const char* data = asio::buffer_cast<const char*>(buf);
	size_t size = asio::buffer_size(buf);

	if (size < HEADER_SIZE)
	{
		return 0;
	}

	int bodyLength = *(int*)(data + HEADER_BODY_LENGTH_OFFSET);

	if (size < bodyLength + HEADER_SIZE)
	{
		return 0;
	}

	return bodyLength + HEADER_SIZE;
}