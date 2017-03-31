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
	_strand.post([this, gameObject]() {
		_gameObjects.insert(GameObjectMapType::value_type(gameObject->GetHandle(), gameObject));
	});
}

void GameWorld::LeaveGameObject(shared_ptr<GameObject>& gameObject)
{
	_strand.post([this, gameObject]() {
		_gameObjects.erase(gameObject->GetHandle());
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


