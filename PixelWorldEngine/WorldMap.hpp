#pragma once

#include "pch.hpp"
#include "Utility.hpp"

namespace PixelWorldEngine {

#define DEFAULT_BLOCK_SIZE 32

	/**
	 * @brief 地图块
	 */
	struct MapData {
		int RenderObjectID; //渲染编号。为0的话，就代表不渲染
		float Opacity; //不透明度，默认为1

		/**
		 * @brief 默认构造函数
		 */
		MapData();
	};

	/**
	 * @brief 当前世界的地图，用于描述世界的地理属性，左上角为地图的 (0,0) 
	 */
	class WorldMap {
	private:
		int width; //世界的宽度
		int height; //世界的高度

		float mapBlockSize; //地图格子的大小，默认为32

		std::string mapName; //世界的地图的名字
	protected:
		std::vector<MapData> mapData; //存储世界的地图数据信息
	public:
		/** 
		 * @brief 构造函数
		 * @param[in] name 世界的地图的名字
		 * @param[in] Width 世界的宽度
		 * @param[in] Height 世界的高度
		 * @param[in] MapBlockSize 世界的格子大小，默认为32
		 * @param[in] data 世界的地图的数据信息，数组大小请保证大于等于Width * Height
		 */
		WorldMap(std::string name, int Width, int Height, float MapBlockSize = DEFAULT_BLOCK_SIZE, MapData* data = nullptr);

		/**
		 * @brief 设置地图指定位置数据信息
		 * @param[in] x 地图的x坐标
		 * @param[in] y 地图的y坐标
		 * @param[in] data 地图数据信息，注意保持数据的生命周期
		 */
		void SetMapData(int x, int y, MapData data);

		/**
		 * @brief 设置地图格子大小
		 * @param[in] 地图格子大小，默认为32
		 */
		void SetMapBlockSize(float size);

		/**
		 * @brief 获取地图指定位置数据信息
		 * @param[in] x 地图的x坐标
		 * @param[in] y 地图的y坐标
		 * @return 数据信息
		 */
		auto GetMapData(int x, int y) -> MapData;

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
		 * @brief 获取地图格子大小
		 * @return 地图格子大小
		 */
		auto GetMapBlockSize() -> float;

		/**
		* @brief 获取点(x, y)对应当前世界地图的格子对应坐标，如果不在地图范围内，则返回 (-1, -1)
		* @param[in] x X坐标
		* @param[in] y Y坐标
		* @return 格子的对应坐标
		*/
		auto GetWorldMapDataIndex(float x, float y) -> std::pair<int, int>;

		/**
		* @brief 获取点(x, y)对应当前世界地图的格子，如果不在地图范围内，则返回nullptr
		* @param[in] x X坐标
		* @param[in] y Y坐标
		* @return 格子的数据
		*/
		auto GetWorldMapData(float x, float y) -> MapData;

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
