#pragma once

#include <wasmtime.hh>
#include <utility>

class WasmRuntime {
public:
	WasmRuntime();
	~WasmRuntime();

	template <typename Func>
	void registerFunction(const std::string& funcName, Func&& func, const std::string& moduleName = "env") {
		m_linker.define(
			m_store,
			moduleName,
			funcName,
			wasmtime::Func::wrap<DecayedFunc>(m_store, std::forward<Func>(func))
		);
	}

private:
	wasmtime::Engine m_engine;
	wasmtime::Store m_store;
	wasmtime::Linker m_linker;
};