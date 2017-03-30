#include "stdafx.h"
#include "CmdReqLogin.h"
#include "Protocol/Protocol.pb.h"
#include "PlayerState.h"
#include "Packet\PacketHelper.h"


void CmdReqLogin::ExecuteCommand(shared_ptr<ISession>& session, shared_ptr<ReqLogin>& message)
{
	auto playerState = (PlayerState&)session->GetSessionState();

	playerState.State = PlayerStateType::Logined;

	ResLogin res;
	res.set_result(PacketResult::SUCCESS);

	PacketHelper::SendPacket(session, PacketCommand::RES_LOGIN, res);
}
