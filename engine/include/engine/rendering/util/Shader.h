#pragma once

#include <SDL3/SDL_gpu.h>

namespace Shader {
	void initializeAssetLoader();

	SDL_GPUShader* loadShader(SDL_GPUDevice* device, const char* shaderFileName);
}