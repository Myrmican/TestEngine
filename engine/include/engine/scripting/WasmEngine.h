#pragma once

#include <wasmtime.hh>
#include <string>

class WasmEngine {
public:
	WasmEngine();
	~WasmEngine();

	bool loadPackage(const std::string& path);

	void callFunction(const std::string& funcName, int parameter);
};