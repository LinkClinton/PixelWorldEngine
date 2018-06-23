#include "Events.hpp"

using namespace PixelWorldEngine::Events;

MouseMoveHandlers PixelWorldEngine::Events::operator+=(MouseMoveHandlers & handlers, MouseMoveHandler handler)
{
	handlers.push_back(handler);
	return handlers;
}

MouseClickHandlers PixelWorldEngine::Events::operator+=(MouseClickHandlers & handlers, MouseClickHandler handler)
{
	handlers.push_back(handler);
	return handlers;
}

MouseWheelHandlers PixelWorldEngine::Events::operator+=(MouseWheelHandlers & handlers, MouseWheelHandler handler)
{
	handlers.push_back(handler);
	return handlers;
}

KeyClickEventHandlers PixelWorldEngine::Events::operator+=(KeyClickEventHandlers & handlers, KeyClickEventHandler handler)
{
	handlers.push_back(handler);
	return handlers;
}

UpdateEventHandlers PixelWorldEngine::Events::operator+=(UpdateEventHandlers & handlers, UpdateEventHandler handler)
{
	handlers.push_back(handler);
	return handlers;
}


