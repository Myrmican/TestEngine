#include <wasmtime.h>
#include <iostream>

namespace WebAssembly {
	void initializeRE() {
		std::cout << "[Engine] Initializing WASM Runtime...\n";

		wasm_engine_t* engine = wasm_engine_new();
		if (!engine) {
			std::cerr << "[Engine Error] Failed to create Wasmtime engine.\n";
			return;
		}

		wasmtime_store_t* store = wasmtime_store_new(engine, nullptr, nullptr);
		wasmtime_context_t* context = wasmtime_store_context(store);

		wasmtime_linker_t* linker = wasmtime_linker_new(engine);


	}
}