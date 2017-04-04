#include "stdafx.h"
#include "CmdReqEnterWorld.h"
#include "GameWorldManager.h"
#include "GameWorld.h"
#include "PlayerState.h"

void CmdReqEnterWorld::ExecuteCommand(shared_ptr<ISession>& session, shared_ptr<ReqEnterWorld>& message)
{
	auto playerState = (PlayerState*)session->GetSessionState();

	auto world = GameWorldManager::GetInstance()->GetWorld(1);

	world->EnterGameObject(playerState->GameObject);

}
