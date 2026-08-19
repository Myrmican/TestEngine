#include <Environment.h>
#include <project/Project.h>
#include <core/Logger.h>

namespace SaveProject {
	void initSave(Project* project) {
		if (project == nullptr) return;

		Logger* logger = project->logger;

		if (!System::IsConnectedToNetwork()) {
			logger->Error("You are not connected to the internet.");
			return;
		}

		logger->Info("Saving the current project...");

		logger->Info("Saved current changes.");
	}
}