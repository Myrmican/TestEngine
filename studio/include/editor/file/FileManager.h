#include <QTabWidget>

namespace Engine {
	class File;

	namespace FileManager {

		void openFile(Engine::File* file, QTabWidget* documentTabs);
	}
}