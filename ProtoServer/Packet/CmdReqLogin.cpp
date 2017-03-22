#include "stdafx.h"
#include "CmdReqLogin.h"
#include "../Protocol/Protocol.pb.h"


void CmdReqLogin::ExecuteCommand(shared_ptr<ISession>& session, asio::const_buffer & buf, int packetLen)
{
	ReqLogin reqLogin;

	if (!reqLogin.ParseFromArray(asio::buffer_cast<const char*>(buf), packetLen))
	{
		LogHelper::GetInstance()->GetConsoleLogger()->error("ReqLogin parse error");
		return;
	}


}
