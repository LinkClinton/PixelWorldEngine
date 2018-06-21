#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 地图某一块的数据
	 */
	struct MapData {
		int RenderObjectID; //表示这一块渲染的纹理ID
	};

	class WorldMap {
	protected:
		MapData* a;
	public:
		
	};

}
