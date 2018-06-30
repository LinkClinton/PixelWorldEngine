#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

#define MAX_RENDER_OBJECT 4

	/**
	 * @brief 地图某一格的数据
	 */
	struct MapData {
		int RenderObjectID[MAX_RENDER_OBJECT]; //表示这一格渲染的纹理ID，我们将会按照顺序(0 -> MAX_RENDER_OBJECT - 1)将其从上到下叠加，但是请注意纹理的Alpha值。为0的话，就代表不使用
		int MoveEnable; //是否允许物体移动到这一格，默认为fasle

		/**
		 * @brief 默认构造函数
		 */
		MapData() = default;
	};

	/**
	 * @brief 当前世界的地图，用于描述世界的地理属性，左上角为地图的 0,0 
	 */
	class WorldMap {
	private:
		int width; //世界的宽度
		int height; //世界的高度

		std::string mapName; //世界的地图的名字
	protected:
		std::vector<MapData*> mapData; //存储世界的地图数据信息
	public:
		/** 
		 * @brief 构造函数
		 * @param[in] name 世界的地图的名字
		 * @param[in] Width 世界的宽度
		 * @param[in] Height 世界的高度
		 * @param[in] data 世界的地图的数据信息，数组大小请保证大于等于Width x Height
		 */
		WorldMap(std::string name, int Width, int Height, MapData** data = nullptr);

		/**
		 * @brief 设置地图指定位置数据信息
		 * @param[in] x 地图的x坐标
		 * @param[in] y 地图的y坐标
		 * @param[in] data 地图数据信息，注意保持数据的生命周期
		 */
		void SetMapData(int x, int y, MapData* data);

		/**
		 * @brief 获取地图指定位置数据信息
		 * @param[in] x 地图的x坐标
		 * @param[in] y 地图的y坐标
		 * @return 数据信息
		 */
		auto GetMapData(int x, int y)->MapData*;

		/**
		 * @brief 获取世界宽度
		 * @return 世界的宽度
		 */
		auto GetWidth() -> int;


		/** 
		 * @brief 获取世界的高度
		 * @return 世界的高度
		 */
		auto GetHeight() -> int;

		/**
		 * @brief 获取世界的地图的名字
		 * @return 世界的地图的名字
		 */
		auto GetMapName() -> std::string;

		/**
		 * @brief 不合法的位置坐标
		 * @return (-1, -1)
		 */
		static auto InvalidLocation() -> std::pair<int, int>;
	};

}
