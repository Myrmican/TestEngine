#include <scripting/ScriptEngine.h>
#include <iostream>

namespace Engine {
	ScriptEngine::ScriptEngine(DataModel* dataModel) {
		m_dataModel = dataModel;

		
	}

	void ScriptEngine::executeFiles() {
		std::vector<Instance*> descendants = m_dataModel->getDescendants();
		for (const Instance* instance : descendants) {
			if (instance->getClassName() != "File") continue;

			std::cout << instance->getName();
		}
	}
}