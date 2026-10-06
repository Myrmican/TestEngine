#include <asset/Package.h>
#include <core/Reflection.h>

namespace Engine {
	REGISTER_INSTANCE(Package, Instance);
	Package::Package() : Instance("Package") {
		
	}

	std::string Package::getAssetReference() const {
		return m_assetReference;
	}

	void Package::setAssetReference(const std::string& reference) {
		m_assetReference = reference;
	}

	void Package::properties(ClassDescriptor* desc) {
		desc->addProperty(new TypedProperty<Package, std::string>(
			"AssetId",
			"Data",
			&Package::getAssetReference,
			nullptr
		));
	}
}