#include "stdafx.h"
#include "PacketDispatcher.h"
#include "Protocol\Protocol.pb.h"
#include "CmdReqLogin.h"



void PacketDispatcher::Initialize()
{
	AddHandler<PacketCommand::REQ_LOGIN, CmdReqLogin>();
}

void PacketDispatcher::Dispatch(shared_ptr<ISession>& session, asio::const_buffer & buf, int packetLen)
{
	
}
