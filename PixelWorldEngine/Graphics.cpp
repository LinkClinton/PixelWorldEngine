#include "Graphics.hpp"

PixelWorldEngine::Graphics::Graphics::Graphics()
{
#ifdef _WIN32

	D3D_FEATURE_LEVEL features[3] = {
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_0
	};

#ifdef DEBUG
	auto createFlag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#else
	auto createFlag = 0;
#endif

	blendEnable = false;
	fillMode = FillMode::FillSolid;

	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		0, createFlag, features, 3, D3D11_SDK_VERSION, &device, &feature, &deviceContext);

	elementDesc[0] = { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 };
	elementDesc[1] = { "COLOR",   0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA ,0 };
	elementDesc[2] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,28,D3D11_INPUT_PER_VERTEX_DATA,0 };

	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.FillMode = (D3D11_FILL_MODE)fillMode;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = blendEnable;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;

	device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	device->CreateBlendState(&blendDesc, &blendState);

	deviceContext->RSSetState(rasterizerState);
	deviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);

#endif // _WIN32

	FT_Init_FreeType(&fontLibrary);
}

void PixelWorldEngine::Graphics::Graphics::SetShader(GraphicsShader* shader)
{

#ifdef _WIN32

	auto vertexShaderCode = shader->GetVertexShaderCode();

	Utility::Dispose(inputLayout);

	device->CreateInputLayout(&elementDesc[0], 3, &vertexShaderCode[0], vertexShaderCode.size(), &inputLayout);

	deviceContext->VSSetShader(shader->vertexShader, nullptr, 0);
	deviceContext->PSSetShader(shader->pixelShader, nullptr, 0);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetVertexBuffer(Buffer* buffer)
{
#ifdef _WIN32
	
	UINT size = buffer->GetSize() / buffer->GetCount();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &buffer->buffer, &size, &offset);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetIndexBuffer(Buffer* buffer)
{

#ifdef _WIN32

	deviceContext->IASetIndexBuffer(buffer->buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetConstantBuffer(Buffer* buffer, int id)
{
	if (buffer == nullptr) return;

#ifdef _WIN32

	deviceContext->VSSetConstantBuffers(id, 1, &buffer->buffer);
	deviceContext->PSSetConstantBuffers(id, 1, &buffer->buffer);

#endif // _WIN32


}

void PixelWorldEngine::Graphics::Graphics::SetShaderResource(ShaderResource* shaderResource, int id)
{
	if (shaderResource == nullptr) return;

#ifdef _WIN32

	deviceContext->VSSetShaderResources(id, 1, &shaderResource->resourceView);
	deviceContext->PSSetShaderResources(id, 1, &shaderResource->resourceView);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetStaticSampler(StaticSampler * staticSampler, int id)
{
	if (staticSampler == nullptr) return;

#ifdef _WIN32

	deviceContext->VSSetSamplers(id, 1, &staticSampler->sampler);
	deviceContext->PSSetSamplers(id, 1, &staticSampler->sampler);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetConstantBuffers(std::vector<Buffer*> buffer, int startID)
{

#ifdef _WIN32

	std::vector<ID3D11Buffer*> buffers(buffer.size());

	for (size_t i = 0; i < buffer.size(); i++)
		buffers[i] = buffer[i]->buffer;

	deviceContext->VSSetConstantBuffers(startID, buffer.size(), &buffers[0]);
	deviceContext->PSSetConstantBuffers(startID, buffer.size(), &buffers[0]);

#endif // _WIN32


}

void PixelWorldEngine::Graphics::Graphics::SetShaderResources(std::vector<ShaderResource*> shaderResource, int startID)
{

#ifdef _WIN32

	std::vector<ID3D11ShaderResourceView*> resourceViews(shaderResource.size());

	for (size_t i = 0; i < shaderResource.size(); i++)
		resourceViews[i] = shaderResource[i]->resourceView;

	deviceContext->VSSetShaderResources(startID, shaderResource.size(), &resourceViews[0]);
	deviceContext->PSSetShaderResources(startID, shaderResource.size(), &resourceViews[0]);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetStaticSampelrs(std::vector<StaticSampler*> staticSampler, int startID)
{

#ifdef _WIN32

	std::vector<ID3D11SamplerState*> staticSamplers(staticSampler.size());

	for (size_t i = 0; i < staticSampler.size(); i++)
		staticSamplers[i] = staticSampler[i]->sampler;

	deviceContext->VSSetSamplers(startID, staticSampler.size(), &staticSamplers[0]);
	deviceContext->PSSetSamplers(startID, staticSampler.size(), &staticSamplers[0]);


#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetRenderTarget(RenderTarget* renderTarget)
{

#ifdef _WIN32

	deviceContext->OMSetRenderTargets(1, &renderTarget->renderTarget, nullptr);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetFillMode(FillMode FillMode)
{
	fillMode = FillMode;

#ifdef _WIN32
	Utility::Dispose(rasterizerState);

	rasterizerDesc.FillMode = (D3D11_FILL_MODE)fillMode;
	
	device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	deviceContext->RSSetState(rasterizerState);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetBlendState(bool state)
{
	blendEnable = state;

#ifdef _WIN32

	Utility::Dispose(blendState);

	blendDesc.RenderTarget[0].BlendEnable = blendEnable;

	device->CreateBlendState(&blendDesc, &blendState);

	deviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::SetViewPort(RectangleF rect)
{

#ifdef _WIN32

	D3D11_VIEWPORT viewPort;

	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = (float)rect.left;
	viewPort.TopLeftY = (float)rect.top;
	viewPort.Width = (float)(rect.right - rect.left);
	viewPort.Height = (float)(rect.bottom - rect.top);

	deviceContext->RSSetViewports(1, &viewPort);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::ClearState()
{
	deviceContext->ClearState();
}

void PixelWorldEngine::Graphics::Graphics::DrawIndexed(int indexCount, int startIndexLocation, int baseVertexLocation)
{

#ifdef _WIN32

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

#endif // _WIN32

}

void PixelWorldEngine::Graphics::Graphics::DrawIndexedInstanced(int indexCount, int instanceCount, int startIndexLocation, int baseVertexLocation)
{

#ifdef _WIN32

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->DrawIndexedInstanced(indexCount, instanceCount, startIndexLocation, baseVertexLocation, 0);

#endif // _WIN32

}

PixelWorldEngine::Graphics::Graphics::~Graphics()
{
#ifdef _WIN32
	Utility::Dispose(device);
	Utility::Dispose(deviceContext);
#endif // _WIN32
	
	FT_Done_FreeType(fontLibrary);
}
