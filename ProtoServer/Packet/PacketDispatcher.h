#pragma once

#include "IPacketCommand.h"


class PacketDispatcher
{
public:
	void Initialize();
	
	void Dispatch(shared_ptr<ISession>& session, asio::const_buffer& buf, int packetLen);

private:
	template <int packetCommand, class PacketCommandClass>
	void AddHandler()
	{
		_handlerMap.insert(PacketMapType::value_type(packetCommand, make_shared<PacketCommandClass>()));
	}


	typedef unordered_map<int, shared_ptr<IPacketCommand>>  PacketMapType;

	PacketMapType _handlerMap;
};