#include <rendering/util/Shader.h>

namespace Shader {
	SDL_GPUShader* loadShader(SDL_GPUDevice* device, const char* name) {
		SDL_GPUShaderCreateInfo shaderInfo = {};
		//shaderInfo.code_size;

		SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);

		return shader;
	}
}