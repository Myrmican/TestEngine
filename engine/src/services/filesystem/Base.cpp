#include <services/filesystem/Base.h>
#include <services/filesystem/FileSystem.h>
#include <fstream>
#include <iostream>

namespace Engine {
	namespace FileSystem {

		void Base::setName(const std::string name) {
			this->name = name;
		}

		Folder* Base::getParent() {
			return nullptr;
		}

		const std::string& Base::getDirectory() {
			return "";
		}

		File::File(const std::string name) {
			std::string path = currentDirectory + "/" + name;
			std::ofstream file(path);
		}

		void File::open() {
			if (internalFile.is_open()) return;

			internalFile.open(filePath);
		}

		bool File::isOpen() {
			if (!internalFile) return false;

			return internalFile.is_open();
		}

		void File::close() {
			if (!internalFile.is_open()) return;

			internalFile.close();
		}
	}
}