#pragma once

#include "IPacketCommand.h"

class CmdReqLogin : public IPacketCommand
{
public:
	void ExecuteCommand(shared_ptr<ISession>& session, asio::const_buffer& buf, int packetLen) override;
};
