#include "stdafx.h"
#include "PacketHelper.h"
#include "Network\FixedHeaderProtocolFilter.h"
#include "Network\Session.h"

void PacketHelper::SendPacket(shared_ptr<ISession>& session, int packetCommand, google::protobuf::Message & message)
{
	int size = message.ByteSize();

	vector<char> buf(size + FixedHeaderProtocolFilter::HeaderSize, 0);

	*(__int16*)&buf[0] = (__int16)packetCommand;
	*(int*)&buf[FixedHeaderProtocolFilter::CommandSize] = size;
	
	if (size > 0)
	{
		message.SerializeToArray(&buf[FixedHeaderProtocolFilter::HeaderSize], size);
	}

	session->Send(buf.data(), (int)buf.size());
}
