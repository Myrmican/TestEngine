#pragma once

#include <datamodel/Instance.h>
#include <core/Event.h>
#include <network/Player.h>
#include <util/Globals.h>

namespace Engine {
	class NetworkSignal : public Creatable {
	public:
		NetworkSignal();

		Event<Player*, const std::vector<Variant>&> OnReceived;

		void Send(Player* target, const std::vector<uint8_t>& payload);
		void Broadcast(const std::vector<uint8_t>& payload);

		static void registerProperties(ClassDescriptor* desc) {};
	};
}