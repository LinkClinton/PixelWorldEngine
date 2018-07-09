#include "ShaderResource.hpp"

#include "Application.hpp"
#include "Graphics.hpp"

PixelWorldEngine::Graphics::Texture2D::Texture2D(Graphics* Graphics, void * Data, int Width, int Height, PixelFormat PixelFormat, int MipLevels)
{
	graphics = Graphics;

	width = Width;
	height = Height;

	pixelFormat = PixelFormat;
	mipLevels = MipLevels;

	rowPitch = width * Utility::CountPixelFormatSize(pixelFormat);
	size = rowPitch * height;

#ifdef _WIN32

	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.Format = (DXGI_FORMAT)pixelFormat;
	desc.Height = height;
	desc.MipLevels = mipLevels;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.Width = width;

	graphics->device->CreateTexture2D(&desc, nullptr, (ID3D11Texture2D**)&resource);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = (DXGI_FORMAT)pixelFormat;
	desc.Texture2D.MipLevels = mipLevels;
	desc.Texture2D.MostDetailedMip = mipLevels - 1;
	desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

	graphics->device->CreateShaderResourceView(resource, &desc, &resourceView);

#endif // _WIN32

	Update(Data);
}

PixelWorldEngine::Graphics::Texture2D::Texture2D(Texture2D * srcTexture, PixelWorldEngine::Rectangle srcRect)
{
	graphics = srcTexture->graphics;

	width = srcRect.right - srcRect.left;
	height = srcRect.bottom - srcRect.top;

	pixelFormat = srcTexture->pixelFormat;
	mipLevels = srcTexture->mipLevels;

	rowPitch = width * Utility::CountPixelFormatSize(pixelFormat);
	size = rowPitch * height;

#ifdef _WIN32

	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.Format = (DXGI_FORMAT)pixelFormat;
	desc.Height = height;
	desc.MipLevels = mipLevels;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	desc.Width = width;

	graphics->device->CreateTexture2D(&desc, nullptr, (ID3D11Texture2D**)&resource);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.Format = (DXGI_FORMAT)pixelFormat;
	desc.Texture2D.MipLevels = mipLevels;
	desc.Texture2D.MostDetailedMip = mipLevels - 1;
	desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

	graphics->device->CreateShaderResourceView(resource, &desc, &resourceView);

	D3D11_BOX box;

	box.left = srcRect.left;
	box.top = srcRect.top;
	box.right = srcRect.right;
	box.bottom = srcRect.bottom;
	box.front = 0;
	box.back = 1;

	graphics->deviceContext->CopySubresourceRegion(resource, 0, 0, 0, 0, srcTexture->resource, 0,
		&box);

#endif // _WIN32

}

PixelWorldEngine::Graphics::Texture2D::~Texture2D()
{

}

void PixelWorldEngine::Graphics::Texture2D::CopyFromTexture2D(Texture2D * srcTexture, int dstPositionX, int dstPositionY, PixelWorldEngine::Rectangle srcRect)
{

#ifdef _WIN32

	D3D11_BOX box;

	box.left = srcRect.left;
	box.top = srcRect.top;
	box.right = srcRect.right;
	box.bottom = srcRect.bottom;
	box.front = 0;
	box.back = 1;

	graphics->deviceContext->CopySubresourceRegion(resource, 0, dstPositionX, dstPositionY, 0, srcTexture->resource, 0,
		&box);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Texture2D::Update(void * data)
{
	if (data == nullptr) return;

#ifdef _WIN32

	graphics->deviceContext->UpdateSubresource(resource,
		0, nullptr, data, rowPitch, 0);

#endif // _WIN32
}

auto PixelWorldEngine::Graphics::Texture2D::GetWidth() -> int
{
	return width;
}

auto PixelWorldEngine::Graphics::Texture2D::GetHeight() -> int
{
	return height;
}

auto PixelWorldEngine::Graphics::Texture2D::GetSize() -> int
{
	return size;
}

auto PixelWorldEngine::Graphics::Texture2D::GetMipLevels() -> int
{
	return mipLevels;
}

auto PixelWorldEngine::Graphics::Texture2D::GetPixelFormat() -> PixelFormat
{
	return pixelFormat;
}

PixelWorldEngine::Graphics::ShaderResource::~ShaderResource()
{
#ifdef _WIN32
	Utility::Dispose(resource);
	Utility::Dispose(resourceView);
#endif // _WIN32

}

PixelWorldEngine::Graphics::BufferArray::BufferArray(Graphics * Graphics, void * data, int dataSize, int dataCount)
{
	graphics = Graphics;

	size = dataSize;
	count = dataCount;

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
