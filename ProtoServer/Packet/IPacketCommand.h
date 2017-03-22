#pragma once

class ISession;

class IPacketCommand
{
public:
	virtual void ExecuteCommand(shared_ptr<ISession>& session, asio::const_buffer& buf, int packet_len) = 0;
};
