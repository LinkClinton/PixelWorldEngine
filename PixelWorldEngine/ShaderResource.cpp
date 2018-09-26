#include "ShaderResource.hpp"

#include "Application.hpp"
#include "Graphics.hpp"

PixelWorldEngine::Graphics::ShaderResource::~ShaderResource()
{
#ifdef _WIN32
	Utility::Dispose(resource);
	Utility::Dispose(resourceView);
#endif // _WIN32

}