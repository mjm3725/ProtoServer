#pragma once

class ISession;

template <class ProtobufPacketType>
class IPacketCommand
{
public:
	virtual void ExecuteCommand(shared_ptr<ISession>& session, shared_ptr<ProtobufPacketType>& message) = 0;
};

