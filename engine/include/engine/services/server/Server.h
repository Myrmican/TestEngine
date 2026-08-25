#pragma once

#include <datamodel/Instance.h>

namespace Engine {
	class Server : public Instance {
	public:
		Server();

		int getPing() const;
		float getPacketLoss() const;

	};
}