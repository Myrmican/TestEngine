#pragma once

#include <datamodel/Instance.h>

namespace Engine {
	class OverlayBase : public Creatable {
	public:
		OverlayBase(std::string_view name);

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