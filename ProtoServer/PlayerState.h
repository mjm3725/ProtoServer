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
	PlayerStateType GetState();

private:
	shared_ptr<GameObject> _gameObject;
	PlayerStateType _state;
};
