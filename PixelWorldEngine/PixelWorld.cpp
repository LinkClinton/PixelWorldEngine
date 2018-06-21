#include "PixelWorld.hpp"

#include "Application.hpp"
#include "EngineDefaultResource.hpp"

PixelWorldEngine::PixelWorld::PixelWorld(std::wstring WorldName, Application * Application)
{
	worldName = WorldName;
	graphics = Application->GetGraphics();

	square = new Graphics::Rectangle(0, 0, 100, 100, graphics);

	buffers.resize((int)BufferIndex::Count);

	auto matrix = glm::mat4(1);

	buffers[(int)BufferIndex::CameraBuffer] = new Graphics::Buffer(graphics, &matrix , sizeof(glm::mat4x4));
	buffers[(int)BufferIndex::TransformBuffer] = new Graphics::Buffer(graphics, &matrix, sizeof(glm::mat4x4));

	defaultShader = new Graphics::GraphicsShader(graphics, 
		Utility::CharArrayToVector((char*)vsPixelWorldDefaultShaderCode),
		Utility::CharArrayToVector((char*)psPixelWorldDefaultShaderCode));
	
	defaultSampler = new Graphics::StaticSampler(graphics);

	SetShader();
}

PixelWorldEngine::PixelWorld::~PixelWorld()
{
	for (size_t i = 0; i < buffers.size(); i++)
		Utility::Delete(buffers[i]);

	Utility::Delete(defaultShader);
	Utility::Delete(renderBuffer);
	Utility::Delete(renderTarget);
	Utility::Delete(renderObject);
}

void PixelWorldEngine::PixelWorld::SetResolution(int width, int height)
{
	if (width == resolutionWidth || height == resolutionHeight) return;

	Utility::Delete(renderBuffer);
	Utility::Delete(renderTarget);
	Utility::Delete(renderObject);

	resolutionWidth = width;
	resolutionHeight = height;

	renderBuffer = new Graphics::Texture2D(graphics, nullptr, resolutionWidth, resolutionHeight,
		Graphics::PixelFormat::R8G8B8A8);

	renderTarget = new Graphics::RenderTarget(graphics, renderBuffer);

	renderObject = new Graphics::Rectangle(0, 0, (float)width, (float)height, graphics);
}

void PixelWorldEngine::PixelWorld::SetCamera(Camera Camera)
{
	camera = Camera;

	auto matrix = camera.GetMatrix();

	buffers[(int)BufferIndex::CameraBuffer]->Update(&matrix);
}

void PixelWorldEngine::PixelWorld::SetShader(Graphics::GraphicsShader * Shader)
{
	shader = Shader;
}

void PixelWorldEngine::PixelWorld::SetShader()
{
	shader = defaultShader;
}

void PixelWorldEngine::PixelWorld::RegisterRenderObjectID(int id, Graphics::Texture2D* texture)
{
	renderObjectIDGroup.insert(std::pair<int, Graphics::Texture2D*>(id, texture));
}

void PixelWorldEngine::PixelWorld::UnRegisterRenderObjectID(int id)
{
	renderObjectIDGroup.erase(id);
}


auto PixelWorldEngine::PixelWorld::GetCurrentWorld() -> Graphics::Texture2D *
{
	renderTarget->Clear(0, 0, 0);

	graphics->ClearState();

	graphics->SetRenderTarget(renderTarget);
	
	graphics->SetViewPort(Rectangle(0.f, 0.f, (float)resolutionWidth, (float)resolutionHeight));

	graphics->SetShader(shader);

	graphics->SetVertexBuffer(square->GetVertexBuffer());
	graphics->SetIndexBuffer(square->GetIndexBuffer());

	graphics->SetConstantBuffers(buffers, 0);
	graphics->SetShaderResource(renderObjectIDGroup[0], 0);
	graphics->SetStaticSampler(defaultSampler, 0);

	graphics->DrawIndexed(square->GetIndexBuffer()->GetCount());

	return renderBuffer;
}





