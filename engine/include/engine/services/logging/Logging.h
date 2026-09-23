#pragma once

#include <datamodel/Instance.h>
#include <core/Event.h>
#include <string>

namespace Engine {
	enum class LoggerMessageType { Info, Warning, Error };

	class Logging : public Instance {
	public:
		Event<std::string_view, LoggerMessageType> messageOut;

		Logging();
	};
}