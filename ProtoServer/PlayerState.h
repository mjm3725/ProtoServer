#pragma once

#include "Network\Session.h"
#include "GameObject.h"


enum class PlayerStateType
{
	None,
	Logined,
	Ready,
	World,
};

class PlayerState : public ISessionState
{
public:
	PlayerStateType State = PlayerStateType::None;
	shared_ptr<GameObject> GameObject;
};
