#pragma once

#include "Protocol/Protocol.pb.h"

class ISession;

class PacketHelper
{
public:
	static void SendPacket(shared_ptr<ISession>& session, int packetCommand, google::protobuf::Message& message);
};
