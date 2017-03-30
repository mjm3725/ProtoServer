#pragma once

#include "IPacketCommand.h"

class ReqReadyEnterWorld;

class CmdReqReadyEnterWorld : public IPacketCommand<ReqReadyEnterWorld>
{
public:
	void ExecuteCommand(shared_ptr<ISession>& session, shared_ptr<ReqReadyEnterWorld>& message) override;

private:
	static atomic_int _handleSeq;
};
