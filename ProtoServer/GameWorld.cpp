#include "stdafx.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "Packet\PacketHelper.h"


GameWorld::GameWorld(int id, asio::io_service& ioService) : _id(id), _strand(ioService)
{
}

int GameWorld::GetId()
{
	return _id;
}

void GameWorld::Update()
{

}

void GameWorld::EnterGameObject(shared_ptr<GameObject>& gameObject)
{
	if (gameObject == nullptr)
	{
		return;
	}

	_strand.post([this, gameObject]() {

		NotifyEnterGameObject notifyOthers;
		gameObject->GetGameObjectInfo(*notifyOthers.add_game_object_infos());
		BroadcastPacket(PacketCommand::NOTIFY_ENTER_GAME_OBJECT, notifyOthers);

		NotifyEnterGameObject notifyMe;
		for (auto& v : _gameObjects)
		{
			v.second->GetGameObjectInfo(*notifyMe.add_game_object_infos());
		}
		gameObject->Send(PacketCommand::NOTIFY_ENTER_GAME_OBJECT, notifyMe);

		_gameObjects.insert(GameObjectMapType::value_type(gameObject->GetHandle(), gameObject));
	});
}

void GameWorld::LeaveGameObject(shared_ptr<GameObject>& gameObject)
{
	if (gameObject == nullptr)
	{
		return;
	}

	_strand.post([this, gameObject]() {
		_gameObjects.erase(gameObject->GetHandle());

		NotifyLeaveGameObject notifyOthers;
		notifyOthers.add_game_object_handles(gameObject->GetHandle());
		BroadcastPacket(PacketCommand::NOTIFY_LEAVE_GAME_OBJECT, notifyOthers);

		NotifyLeaveGameObject notifyMe;
		for (auto& v : _gameObjects)
		{
			notifyMe.add_game_object_handles(v.second->GetHandle());
		}
		gameObject->Send(PacketCommand::NOTIFY_LEAVE_GAME_OBJECT, notifyMe);
	});
}

void GameWorld::BroadcastPacket(int packetCommand, google::protobuf::Message& message)
{
	vector<char> buffer;
	PacketHelper::MakePacket(packetCommand, message, buffer);

	for (auto& v : _gameObjects)
	{
		v.second->Send(buffer.data(), (int)buffer.size());
	}
}


