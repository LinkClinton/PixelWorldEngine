#include "Texture2DArray.hpp"

#include "Application.hpp"
#include "Graphics.hpp"

PixelWorldEngine::Graphics::Texture2DArray::Texture2DArray(Graphics * Graphics, int Width, int Height, int ArraySize, PixelFormat PixelFormat, int MipLevels)
{
	graphics = Graphics;

	width = Width;
	height = Height;

	pixelFormat = PixelFormat;
	mipLevels = MipLevels;

	arraySize = ArraySize;

	rowPitch = width * Utility::CountPixelFormatSize(pixelFormat);
	size = rowPitch * height;

	if (size == 0) return;

	DebugReturn(DebugLayer::Assert(size < 0, Error::TheValueIsNotRight, "Width * Height", FunctionName));
	DebugReturn(DebugLayer::Assert(arraySize < 0, Error::TheValueIsNotRight, "ArraySize", FunctionName));

#ifdef _WIN32

	desc.ArraySize = arraySize;
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
	desc.Texture2DArray.ArraySize = arraySize;
	desc.Texture2DArray.FirstArraySlice = 0;
	desc.Texture2DArray.MipLevels = mipLevels;
	desc.Texture2DArray.MostDetailedMip = mipLevels - 1;
	desc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DARRAY;

	graphics->device->CreateShaderResourceView(resource, &desc, &resourceView);

#endif // _WIN32
}

PixelWorldEngine::Graphics::Texture2DArray::~Texture2DArray()
{
}

void PixelWorldEngine::Graphics::Texture2DArray::CopyFromTexture2D(Texture2D * srcTexture, int dstPositionX, int dstPositionY, int arrayIndex, PixelWorldEngine::Rectangle srcRect)
{
	if (srcRect.right - srcRect.left == 0 || srcRect.bottom - srcRect.top == 0) return;

	DebugReturn(DebugLayer::Assert(srcRect.right - srcRect.left < 0 || srcRect.bottom - srcRect.top < 0,
		Error::TheValueIsNotRight, "srcRect", FunctionName));
	DebugReturn(DebugLayer::Assert(arrayIndex < 0 || arrayIndex >= arraySize, Error::TheValueIsNotRight, "arrayIndex", FunctionName));

#ifdef _WIN32

	D3D11_BOX box;

	box.left = srcRect.left;
	box.top = srcRect.top;
	box.right = srcRect.right;
	box.bottom = srcRect.bottom;
	box.front = 0;
	box.back = 1;

	graphics->deviceContext->CopySubresourceRegion(resource, arrayIndex, dstPositionX, dstPositionY, 0, srcTexture->resource, 0,
		&box);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Texture2DArray::CopyFromTexture2DArray(Texture2DArray * srcTextureArray, int dstArrayStartIndex, int srcArrayStartIndex, int arraySize)
{
#ifdef _WIN32

	D3D11_BOX box;

	box.left = 0;
	box.top = 0;
	box.right = srcTextureArray->GetWidth();
	box.bottom = srcTextureArray->GetHeight();
	box.front = 0;
	box.back = 1;

	graphics->deviceContext->CopySubresourceRegion(resource, dstArrayStartIndex, 0, 0, srcArrayStartIndex, srcTextureArray->resource, srcArrayStartIndex,
		&box);

#endif // _WIN32
}

void PixelWorldEngine::Graphics::Texture2DArray::Update(void * data, int arrayIndex)
{
	if (data == nullptr) return;

	DebugReturn(DebugLayer::Assert(arrayIndex < 0 || arrayIndex >= arraySize, Error::TheValueIsNotRight, "arrayIndex", FunctionName));

#ifdef _WIN32

	graphics->deviceContext->UpdateSubresource(resource,
		arrayIndex, nullptr, data, rowPitch, 0);

#endif // _WIN32
}

auto PixelWorldEngine::Graphics::Texture2DArray::GetWidth() -> int
{
	return width;
}

auto PixelWorldEngine::Graphics::Texture2DArray::GetHeight() -> int
{
	return height;
}

auto PixelWorldEngine::Graphics::Texture2DArray::GetSize() -> int
{
	return size;
}

auto PixelWorldEngine::Graphics::Texture2DArray::GetArraySize() -> int
{
	return arraySize;
}

auto PixelWorldEngine::Graphics::Texture2DArray::GetMipLevels() -> int
{
	return mipLevels;
}

auto PixelWorldEngine::Graphics::Texture2DArray::GetPixelFormat() -> PixelFormat
{
	return pixelFormat;
}

