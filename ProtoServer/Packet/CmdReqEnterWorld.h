#pragma once

#include "IPacketCommand.h"

class ReqEnterWorld;

class CmdReqEnterWorld : public IPacketCommand<ReqEnterWorld>
{
public:
	void ExecuteCommand(shared_ptr<ISession>& session, shared_ptr<ReqEnterWorld>& message) override;
};