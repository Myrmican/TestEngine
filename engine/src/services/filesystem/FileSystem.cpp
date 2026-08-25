#include <services/filesystem/FileSystem.h>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

namespace Engine {
	namespace FileSystem {
		static void changeDirectory(const std::string directoryPath) {
			currentDirectory = directoryPath;
		}

		static File* createFile(const std::string name) {
			File* fileObject = new File(name);
			return fileObject;
		}

		static bool deleteFile(const std::string name) {
			std::string path = currentDirectory + "/" + name;
			
			std::error_code ec;
			if (fs::remove(path, ec)) {
				std::cout << "File successfully deleted.\n";
			}
			else {
				std::cout << "Delete failed: " << ec.message() << '\n';
			}
		}
	}
}