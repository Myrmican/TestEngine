#include <datamodel/Instance.h>
#include <core/Event.h>
#include <string>

namespace Engine {
	enum class MessageType { Info, Warning, Error };

	class LogService : Instance {
	public:
		Event<const std::string&, MessageType> messageOut;
	};
}