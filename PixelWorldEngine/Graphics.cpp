#include "Graphics.hpp"

PixelWorldEngine::Graphics::Graphics::Graphics()
{
#ifdef WINDOWS

	D3D_FEATURE_LEVEL features[4] = {
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_0
	};

#ifdef DEBUG
	auto createFlag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#else
	auto createFlag = 0;
#endif

	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		0, createFlag, features, 4, D3D11_SDK_VERSION, &device, &feature, &deviceContext);

	elementDesc[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 };
	elementDesc[1] = { "COLOR",   0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA ,0 };
	elementDesc[2] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,28,D3D11_INPUT_PER_VERTEX_DATA,0 };

	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;

#endif // WINDOWS
}

void PixelWorldEngine::Graphics::Graphics::SetShader(GraphicsShader* shader)
{

#ifdef WINDOWS

	auto vertexShaderCode = shader->GetVertexShaderCode();

	device->CreateInputLayout(&elementDesc[0], 3, &vertexShaderCode[0], vertexShaderCode.size(), &inputLayout);

	deviceContext->VSSetShader(shader->vertexShader, nullptr, 0);
	deviceContext->PSSetShader(shader->pixelShader, nullptr, 0);

#endif // WINDOWS

}

void PixelWorldEngine::Graphics::Graphics::SetVertexBuffer(Buffer* buffer)
{
#ifdef WINDOWS
	
	UINT size = buffer->GetSize() / buffer->GetCount();

	deviceContext->IASetVertexBuffers(0, 1, &buffer->buffer, &size, nullptr);

#endif // WINDOWS

}

void PixelWorldEngine::Graphics::Graphics::SetIndexBuffer(Buffer* buffer)
{

#ifdef WINDOWS

	deviceContext->IASetIndexBuffer(buffer->buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

#endif // WINDOWS

}

void PixelWorldEngine::Graphics::Graphics::SetConstantBuffer(Buffer* buffer, int id)
{

#ifdef WINDOWS

	deviceContext->VSSetConstantBuffers(id, 1, &buffer->buffer);
	deviceContext->PSSetConstantBuffers(id, 1, &buffer->buffer);

#endif // WINDOWS


}

void PixelWorldEngine::Graphics::Graphics::SetShaderResource(ShaderResource* shaderResource, int id)
{

#ifdef WINDOWS

	deviceContext->VSSetShaderResources(id, 1, &shaderResource->resourceView);
	deviceContext->PSSetShaderResources(id, 1, &shaderResource->resourceView);

#endif // WINDOWS

}

void PixelWorldEngine::Graphics::Graphics::SetConstantBuffers(std::vector<Buffer*> buffer, int startID)
{

#ifdef WINDOWS

	std::vector<ID3D11Buffer*> buffers(buffer.size());

	for (size_t i = 0; i < buffer.size(); i++)
		buffers[i] = buffer[i]->buffer;

	deviceContext->VSSetConstantBuffers(startID, buffer.size(), &buffers[0]);
	deviceContext->PSSetConstantBuffers(startID, buffer.size(), &buffers[0]);

#endif // WINDOWS


}

void PixelWorldEngine::Graphics::Graphics::SetShaderResources(std::vector<ShaderResource*> shaderResource, int startID)
{

#ifdef WINDOWS

	std::vector<ID3D11ShaderResourceView*> resourceViews(shaderResource.size());

	for (size_t i = 0; i < shaderResource.size(); i++)
		resourceViews[i] = shaderResource[i]->resourceView;

	deviceContext->VSSetShaderResources(startID, shaderResource.size(), &resourceViews[0]);
	deviceContext->PSSetShaderResources(startID, shaderResource.size(), &resourceViews[0]);

#endif // WINDOWS

}

void PixelWorldEngine::Graphics::Graphics::SetRenderTarget(RenderTarget* renderTarget)
{

#ifdef WINDOWS

	deviceContext->OMSetRenderTargets(1, &renderTarget->renderTarget, nullptr);

#endif // WINDOWS

}

void PixelWorldEngine::Graphics::Graphics::SetFillMode(FillMode fillMode)
{

#ifdef WINDOWS
	Utility::Dipose(rasterizerState);

	rasterizerDesc.FillMode = (D3D11_FILL_MODE)fillMode;
	
	device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	deviceContext->RSSetState(rasterizerState);

#endif // WINDOWS

}

void PixelWorldEngine::Graphics::Graphics::SetBlendState(bool state)
{
	
#ifdef WINDOWS

	Utility::Dipose(blendState);

	blendDesc.RenderTarget[0].BlendEnable = state;

	device->CreateBlendState(&blendDesc, &blendState);

	deviceContext->OMSetBlendState(blendState, nullptr, 0);

#endif // WINDOWS

}

void PixelWorldEngine::Graphics::Graphics::DrawIndexed(int indexCount, int startIndexLocation, int baseVertexLocation)
{

#ifdef WINDOWS

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

#endif // WINDOWS

}

PixelWorldEngine::Graphics::Graphics::~Graphics()
{
#ifdef WINDOWS
	Utility::Dipose(device);
	Utility::Dipose(deviceContext);
#endif // WINDOWS
}