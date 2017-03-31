#pragma once

#include <concurrent_queue.h>
#include "Protocol\Protocol.pb.h"


class GameObject;

class GameWorld
{
public:
	GameWorld(int id, asio::io_service& ioService);

	int GetId();

	void Update();


	void EnterGameObject(shared_ptr<GameObject>& gameObject);
	void LeaveGameObject(shared_ptr<GameObject>& gameObject);

private:
	void BroadcastPacket(int packetCommand, google::protobuf::Message& message);

	asio::io_service::strand _strand;
	chrono::time_point<chrono::system_clock> _lastTime;
	int _id;

	typedef unordered_map<__int64, shared_ptr<GameObject>> GameObjectMapType;
	GameObjectMapType _gameObjects;
};