#include <core/Event.h>
#include <network/Player.h>
#include <vector>

namespace Engine {
	class Anticheat : public Instance {
	public:
		Event<Player*, int> flagged;

		Anticheat();

		static void properties(ClassDescriptor* desc) {};
	};
}