#include "stdafx.h"
#include "FixedHeaderProtocolFilter.h"


int FixedHeaderProtocolFilter::Parse(asio::const_buffer& buf)
{
	const char* data = asio::buffer_cast<const char*>(buf);
	size_t size = asio::buffer_size(buf);

	if (size < HeaderSize)
	{
		return 0;
	}

	int bodyLength = *(int*)(data + BodyLengthOffset);

	if (size < bodyLength + HeaderSize)
	{
		return 0;
	}

	return bodyLength + HeaderSize;
}