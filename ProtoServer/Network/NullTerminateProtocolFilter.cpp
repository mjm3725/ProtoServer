#include "stdafx.h"
#include "NullTerminateProtocolFilter.h"

int NullTerminateProtocolFilter::Parse(asio::const_buffer& buf)
{
	const char* data = asio::buffer_cast<const char*>(buf);
	size_t size = asio::buffer_size(buf);

	for (int i = 0; i < size; i++)
	{
		if (data[i] == '\0')
		{
			return i + 1;
		}
	}

	return 0;
}
