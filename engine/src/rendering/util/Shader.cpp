#include <rendering/util/Shader.h>
#include <SDL3/SDL_filesystem.h>
#include <iostream>

namespace Shader {

	static const char* BasePath = NULL;

	void initializeAssetLoader()
	{
		BasePath = SDL_GetBasePath();
	}

	SDL_GPUShader* loadShader(SDL_GPUDevice* device, const char* fileName) {
		SDL_GPUShaderStage stage;
		if (SDL_strstr(fileName, ".vert")) {
			stage = SDL_GPU_SHADERSTAGE_VERTEX;
		} else if (SDL_strstr(fileName, ".frag")) {
			stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
		}
		else {
			std::cout << "Invalid shader stage!";
			return NULL;
		}

		char fullPath[256];
		SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
		SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
		const char* entrypoint;

		if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
			SDL_snprintf(fullPath, sizeof(fullPath), "%sassets/shaders/compiled/SPIRV/%s.spv", BasePath, fileName);
			format = SDL_GPU_SHADERFORMAT_SPIRV;
			entrypoint = "main";
		}
		else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
			SDL_snprintf(fullPath, sizeof(fullPath), "%sassets/shaders/compiled/MSL/%s.msl", BasePath, fileName);
			format = SDL_GPU_SHADERFORMAT_MSL;
			entrypoint = "main0";
		}
		else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
			SDL_snprintf(fullPath, sizeof(fullPath), "%sassets/shaders/compiled/DXIL/%s.dxil", BasePath, fileName);
			format = SDL_GPU_SHADERFORMAT_DXIL;
			entrypoint = "main";
		}
		else {
			std::cout << "%s", "Unrecognized backend shader format!";
			return NULL;
		}

		size_t codeSize;
		void* code = SDL_LoadFile(fullPath, &codeSize);
		if (code == NULL)
		{
			std::cout << "Failed to load shader from disk! " << fullPath << std::endl;
			return NULL;
		}

		SDL_GPUShaderCreateInfo shaderInfo = {};
		shaderInfo.code = static_cast<const Uint8*>(code);
		shaderInfo.code_size = codeSize;
		shaderInfo.entrypoint = entrypoint;
		shaderInfo.stage = stage;

		SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
		if (shader == NULL) {
			std::cout << "Failed to create shader!" << std::endl;
			SDL_free(code);
			return NULL;
		}

		SDL_free(code);
		return shader;
	}
}