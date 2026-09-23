#include <variant>
#include <string>

namespace Engine {
	class Instance;

	using Variant = std::variant<
		std::monostate,
		bool,
		double,
		std::string,
		Instance*
	>;
}