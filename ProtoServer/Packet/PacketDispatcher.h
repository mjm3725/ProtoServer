#pragma once

#include "IPacketCommand.h"
#include "Network\FixedHeaderProtocolFilter.h"


class PacketDispatcher
{
public:
	void Initialize();
	
	void Dispatch(shared_ptr<ISession>& session, asio::const_buffer& buf, int packetLen);

private:
	template <int packetCommand, class PacketCommandClass, class ProtobufPacketType>
	void AddHandler()
	{
		_handlerMap.insert(PacketMapType::value_type(packetCommand, [](shared_ptr<ISession>& session, asio::const_buffer& buf, int packetLen) {

			auto packet = make_shared<ProtobufPacketType>();
			
			if (!packet->ParseFromArray(asio::buffer_cast<const char*>(buf) + FixedHeaderProtocolFilter::HeaderSize, packetLen - FixedHeaderProtocolFilter::HeaderSize))
			{
				LogHelper::GetInstance()->GetConsoleLogger()->error("packet parse fail: {}", packetCommand);
				return;
			}

			PacketCommandClass packetCommandInstance;
			packetCommandInstance.ExecuteCommand(session, packet);
		}));
	}


	typedef unordered_map<int, function<void(shared_ptr<ISession>&, asio::const_buffer&, int)>> PacketMapType;

	PacketMapType _handlerMap;
};