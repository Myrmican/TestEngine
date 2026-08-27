#include <services/filesystem/Base.h>

namespace Engine {
	namespace FileSystem {
		inline std::string currentDirectory;

		static void changeDirectory(const std::string directoryPath);

		File* createFile(const std::string name);
		bool deleteFile(const std::string name);
	}
}