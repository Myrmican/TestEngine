#pragma once

#include <string>

namespace Engine {

	enum class LogLevel { Info, Warning, Error };

	struct LogEntry {
		LogLevel level;
		std::string message;
	};

	class Logger {
	public:
		void info(std::string_view message);
		void warn(std::string_view message);
		void error(std::string_view message);
	};
}