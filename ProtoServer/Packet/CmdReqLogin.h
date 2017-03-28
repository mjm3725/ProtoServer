#pragma once

#include "IPacketCommand.h"

class ReqLogin;

class CmdReqLogin : public IPacketCommand<ReqLogin>
{
public:
	void ExecuteCommand(shared_ptr<ISession>& session, shared_ptr<ReqLogin>& message) override;
};
