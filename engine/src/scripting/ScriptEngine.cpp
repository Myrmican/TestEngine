#include <scripting/ScriptEngine.h>
#include <datamodel/Instance.h>
#include <datamodel/instances/File.h>
#include <iostream>

namespace Engine {
	ScriptEngine::ScriptEngine(DataModel* dataModel) {
		m_dataModel = dataModel;

		
	}

	void ScriptEngine::executeFiles() {
		std::vector<Instance*> descendants = m_dataModel->getDescendants();
		for (Instance* instance : descendants) {
			File* file = dynamic_cast<File*>(instance);
			if (instance->getClassName() != "File") continue;

			std::string fileContent = file->getContent();
		}
	}
}