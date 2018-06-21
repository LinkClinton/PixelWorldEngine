#include "Geometry.hpp"

PixelWorldEngine::Graphics::Geometry::Geometry(Graphics * Graphics)
{
	graphics = Graphics;

	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}

PixelWorldEngine::Graphics::Geometry::~Geometry()
{
	delete vertexBuffer;
	delete indexBuffer;
}

auto PixelWorldEngine::Graphics::Geometry::GetVertexBuffer() -> Buffer *
{
	return vertexBuffer;
}

auto PixelWorldEngine::Graphics::Geometry::GetIndexBuffer() -> Buffer *
{
	return indexBuffer;
}

PixelWorldEngine::Graphics::RectangleF::RectangleF(float left, float top, float right, float bottom,
	Graphics * graphics) :Geometry(graphics)
{
	//left-top left-bottom right-bottom right-top
	Vertex vertics[4];

	vertics[0].SetPosition(left, top, 1);
	vertics[1].SetPosition(left, bottom, 1);
	vertics[2].SetPosition(right, bottom, 1);
	vertics[3].SetPosition(right, top, 1);

	vertics[0].SetTexturePosition1(0, 0);
	vertics[1].SetTexturePosition1(0, 1);
	vertics[2].SetTexturePosition1(1, 1);
	vertics[3].SetTexturePosition1(1, 0);

	int indices[6];

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 2;
	indices[4] = 0;
	indices[5] = 3;

	vertexBuffer = new Buffer(graphics, vertics, sizeof(vertics), 4, BufferType::VertexBuffer);
	indexBuffer = new Buffer(graphics, indices, sizeof(indices), 6, BufferType::IndexBuffer);
}

