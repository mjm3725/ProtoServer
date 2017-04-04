#include "stdafx.h"
#include "PacketHelper.h"
#include "Network\FixedHeaderProtocolFilter.h"
#include "Network\Session.h"
#include "GameObject.h"


void PacketHelper::SendPacket(shared_ptr<ISession>& session, int packetCommand, google::protobuf::Message& message)
{
	vector<char> buffer;
	MakePacket(packetCommand, message, buffer);
	session->Send(buffer.data(), (int)buffer.size());
}

void PacketHelper::MakePacket(int packetCommand, google::protobuf::Message& message, vector<char>& buffer)
{
	int size = message.ByteSize();

	buffer.resize(size + FixedHeaderProtocolFilter::HeaderSize);

	*(__int16*)&buffer[0] = (__int16)packetCommand;
	*(int*)&buffer[FixedHeaderProtocolFilter::CommandSize] = size;

	if (size > 0)
	{
		message.SerializeToArray(&buffer[FixedHeaderProtocolFilter::HeaderSize], size);
	}
}
