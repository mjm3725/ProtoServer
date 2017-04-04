#include "stdafx.h"
#include "CmdReqReadyEnterWorld.h"
#include "PlayerState.h"
#include "Protocol/Protocol.pb.h"
#include "Packet\PacketHelper.h"

atomic_int CmdReqReadyEnterWorld::_handleSeq = 0;

void CmdReqReadyEnterWorld::ExecuteCommand(shared_ptr<ISession>& session, shared_ptr<ReqReadyEnterWorld>& message)
{
	auto playerState = (PlayerState*)session->GetSessionState();

	playerState->State = PlayerStateType::Ready;
	
	__int64 handle = ++_handleSeq;
	char name[50];
	_i64toa_s(handle, name, 50, 10);

	playerState->GameObject = make_shared<GameObject>(handle, (string)name);
	playerState->GameObject->SetSession(session);

	ResReadyEnterWorld res;
	res.set_result(PacketResult::SUCCESS);
	res.mutable_game_object_info()->set_handle(handle);
	res.mutable_game_object_info()->set_name(name);
	res.mutable_game_object_info()->mutable_pos()->set_x((float)(rand() % 150 + 50));
	res.mutable_game_object_info()->mutable_pos()->set_z((float)(rand() % 150 + 50));

	PacketHelper::SendPacket(session, PacketCommand::RES_READY_ENTER_WORLD, res);
}
