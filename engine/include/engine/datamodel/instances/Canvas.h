#pragma once

#include <datamodel/instances/OverlayBase.h>

namespace Engine {
	class Canvas : public OverlayBase {
	public:
		Canvas(std::string_view name = "Canvas");

		bool isEnabled() const { return m_enabled; }

		void setEnabled(const bool enabled) {
			if (m_enabled != enabled) {
				changing.call("Enabled", enabled);
				m_enabled = enabled;
				changed.call("Enabled");

			}
		}

		static void properties(ClassDescriptor* desc);

	private:
		bool m_enabled = true;
	};
}