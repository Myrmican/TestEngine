#pragma once

#include <datamodel/Instance.h>
#include <string>

namespace Engine {
	class Package : public Instance {
	public:
		Package();

		std::string getAssetReference() const;

		void setAssetReference(const std::string& reference);

		static void properties(ClassDescriptor* desc);

	private:
		std::string m_assetReference;
	};
}