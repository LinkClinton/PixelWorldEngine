#include "StaticSampler.hpp"
#include "Graphics.hpp"

PixelWorldEngine::Graphics::StaticSampler::StaticSampler(Graphics* Graphics, TextureAddressMode addressU,
	TextureAddressMode addressV, TextureAddressMode addressW, TextureFilter filter)
{
	graphics = Graphics;

	AddressU = addressU;
	AddressV = addressV;
	AddressW = addressW;

	Filter = filter;

#ifdef _WIN32
	D3D11_SAMPLER_DESC desc;

	desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)AddressU;
	desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)AddressV;
	desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)AddressW;
	desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	desc.Filter = (D3D11_FILTER)Filter;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 4;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[1] = desc.BorderColor[2] = 1;

	graphics->device->CreateSamplerState(&desc, &sampler);
#endif // _WIN32



}

PixelWorldEngine::Graphics::StaticSampler::StaticSampler(Graphics* Graphics, TextureAddressMode addressUVW, TextureFilter filter)
{
	graphics = Graphics;

	AddressU = AddressV = AddressW = addressUVW;

	Filter = filter;

#ifdef _WIN32
	D3D11_SAMPLER_DESC desc;

	desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)AddressU;
	desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)AddressV;
	desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)AddressW;
	desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	desc.Filter = (D3D11_FILTER)Filter;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 4;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 1;


	graphics->device->CreateSamplerState(&desc, &sampler);
#endif // _WIN32
}

PixelWorldEngine::Graphics::StaticSampler::~StaticSampler()
{
#ifdef _WIN32
	Utility::Dispose(sampler);
#endif // _WIN32

}
