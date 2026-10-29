#pragma once

#include <SDL3/SDL_gpu.h>

namespace Shader {
	SDL_GPUShader* loadShader(SDL_GPUDevice* device, const char* shaderFileName);
}