#pragma once

#include <datamodel/DataModel.h>
#include <datamodel/instances/File.h>
#include <vector>

namespace Engine {
	class ScriptEngine {
	public:
		ScriptEngine(DataModel* dataModel);

		void executeFiles();
	private:
		DataModel* m_dataModel = nullptr;

	};
}