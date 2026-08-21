#pragma once
#include <string_view>

class Output;

class Logger {
public:
	Output* outputDock;

	Logger(Output* parent);
	void Info(std::string_view message);
	void Error(std::string_view message);
};