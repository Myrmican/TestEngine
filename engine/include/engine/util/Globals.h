#include <variant>
#include <string>
#include <core/math/Color3.h>
#include <core/math/Vector3.h>
#include <datamodel/Instance.h>

namespace Engine {
	namespace Globals {
        class Instance;

        using Variant = std::variant<
            std::monostate,
            std::string,
            std::string_view,
            Engine::Instance*,
            Color3,
            Vector3,
            float,
            double,
            int,
            bool
        >;
	}
}