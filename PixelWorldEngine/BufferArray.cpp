#include "BufferArray.hpp"

#include "Application.hpp"
#include "Graphics.hpp"

PixelWorldEngine::Graphics::BufferArray::BufferArray(Graphics * Graphics, void * data, int dataSize, int dataCount)
{
	graphics = Graphics;

	size = dataSize;
	count = dataCount;

	if (dataSize == 0) return;

	DebugReturn(DebugLayer::Assert(dataSize < 0, Error::TheValueIsNotRight, "dataSize", FunctionName));
	DebugReturn(DebugLayer::Assert(dataCount <= 0, Error::TheValueIsNotRight, "dataCount", FunctionName));

#ifdef _WIN32

	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = size;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = size / count;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	graphics->device->CreateBuffer(&desc, nullptr, &buffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = count;
	desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_BUFFER;

	graphics->device->CreateShaderResourceView(buffer, &desc, &resourceView);
#endif // _WIN32

	Update(data);
}

PixelWorldEngine::Graphics::BufferArray::~BufferArray()
{
	Utility::Dispose(buffer);
}

void PixelWorldEngine::Graphics::BufferArray::Update(void * data)
{
	if (data == nullptr) return;

	graphics->deviceContext->UpdateSubresource(buffer, 0, nullptr,
		data, 0, 0);
}

auto PixelWorldEngine::Graphics::BufferArray::GetSize() -> int
{
	return size;
}

auto PixelWorldEngine::Graphics::BufferArray::GetCount() -> int
{
	return count;
}