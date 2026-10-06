#include <datamodel/instances/Model.h>
#include <core/Reflection.h>

#include <string>

namespace Engine {
	REGISTER_INSTANCE(Model, Instance);
	Model::Model(std::string name) : Creatable(name) {
	}

	void Model::pivotTo() {
		
	}

	void Model::properties(Engine::ClassDescriptor* desc) {

	}
}