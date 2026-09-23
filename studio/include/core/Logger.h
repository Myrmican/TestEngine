#pragma once
#include <string_view>

class Output;
class Project;

class Logger {
public:
	Output* outputDock;
	Project* project;

	Logger(Output* parent, Project* project);
	void Info(std::string_view message);
	void Error(std::string_view message);
};