#include <concepts>
#include <string>
#include <string_view>

namespace Engine {
	namespace CheckType {
		template <typename T>
		concept StringLike = std::is_same_v<std::decay_t<T>, std::string> ||
			std::is_same_v<std::decay_t<T>, std::string_view> ||
			std::is_convertible_v<T, std::string_view>;
	}
}