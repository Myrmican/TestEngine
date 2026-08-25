#pragma once

#include <boost/flyweight.hpp>
#include <fstream>
#include <string>

namespace Engine {
	namespace FileSystem {

		class Folder;

		class Base {
		public:
			const std::string& getName() { return name.get(); }
			void setName(const std::string name);

			const std::string& getDirectory();
			Folder* getParent();
		private:
			boost::flyweight<std::string> name;
		};

		class Folder : public Base {
		public:
			int getFiles();
		};

		class File : public Base {
		public:
			File(const std::string name);

			std::string getContent();
			void setContent();

			void open();
			bool isOpen();
			void close();

		private:
			std::string filePath;
			std::fstream internalFile;
		};
	}
}