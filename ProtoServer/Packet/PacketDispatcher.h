#pragma once

#include "IPacketCommand.h"

class PacketDispatcher
{
public:

private:
	unorderd_map<int, shared_ptr<IPacketCommand>> _packetMap;

};