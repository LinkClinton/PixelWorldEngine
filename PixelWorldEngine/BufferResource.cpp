#include "BufferResource.hpp"

#include "Application.hpp"
#include "Graphics.hpp"

PixelWorldEngine::Graphics::Buffer::Buffer(Graphics* Graphics, void * data, int dataSize, int dataCount, BufferType bufferType)
{
	graphics = Graphics;

	size = dataSize;
	count = dataCount;
	type = bufferType;

	if (dataSize == 0) return;

	DebugReturn(DebugLayer::Assert(dataSize < 0, Error::TheValueIsNotRight, "dataSize", FunctionName));
	DebugReturn(DebugLayer::Assert(dataCount <= 0, Error::TheValueIsNotRight, "dataCount", FunctionName));
#ifdef _WIN32

	
	desc.BindFlags = (D3D11_BIND_FLAG)type;
	desc.ByteWidth = size;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = size / count;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	graphics->device->CreateBuffer(&desc, nullptr, &buffer);

#endif // _WIN32

	Update(data);
}

PixelWorldEngine::Graphics::Buffer::~Buffer() 
{
#ifdef _WIN32
	Utility::Dispose(buffer);
#endif // _WIN32

}

void PixelWorldEngine::Graphics::Buffer::Update(void * data)
{
	if (data == nullptr) return;

	graphics->deviceContext->UpdateSubresource(buffer, 0, nullptr,
		data, 0, 0);
}

auto PixelWorldEngine::Graphics::Buffer::GetSize() -> int
{
	return size;
}

auto PixelWorldEngine::Graphics::Buffer::GetCount() -> int
{
	return count;
}

auto PixelWorldEngine::Graphics::Buffer::GetType() -> BufferType
{
	return type;
}



