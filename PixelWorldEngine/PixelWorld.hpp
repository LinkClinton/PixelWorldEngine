#pragma once

#include "pch.hpp"

#include "DataManager.hpp"
#include "Graphics.hpp"
#include "Events.hpp"

#include "WorldMap.hpp"
#include "PixelObject.hpp"
#include "Geometry.hpp"
#include "Utility.hpp"
#include "Camera.hpp"

namespace PixelWorldEngine {

	/**
	 * 用来表述用途不同的缓冲的在着色器以及数组的ID
	 */
	enum class BufferIndex : int {
		CameraBuffer, //摄像机矩阵缓冲
		TransformBuffer, //变换矩阵缓冲
		RenderConfig, //记录当前渲染的时候的设置
		Count
	};

	struct PixelWorldRenderConfig {
		int currentRenderObjectID[MAX_RENDER_OBJECT];
		glm::vec4 unused[3];
	};

	/**
	 * @brief 引擎的核心世界
	 */
	class PixelWorld {
	private:
		std::string worldName; //世界的名字 

		int resolutionWidth; //分辨率宽度
		int resolutionHeight; //分辨率高度

		float renderObjectSize; //每一个地图块的大小，默认值为32

		PixelWorldRenderConfig renderConfig; //渲染设置数据

		Camera* camera; //摄像机

		Graphics::Graphics* graphics; //...

		Graphics::Texture2D* renderBuffer; //存储世界被渲染后的像素信息
		Graphics::RenderTarget* renderTarget; //..

		Graphics::GraphicsShader* defaultShader; //默认着色器
		Graphics::GraphicsShader* shader; //使用的着色器

		std::vector<Graphics::Buffer*> buffers; //缓冲数组

		Graphics::RectangleF* renderObject; //正方形
		Graphics::RectangleF* renderCanvas; //画布

		Graphics::StaticSampler* defaultSampler; //默认的采样器

		WorldMap* worldMap; //当前使用的地图，默认为空

		std::map<int, Graphics::Texture2D*> renderObjectIDGroup; //用于存储纹理，不同的ID对应不同的纹理
	
		std::map<std::string, WorldMap*> worldMaps; //存储世界的地图
		std::map<std::string, PixelObject*> pixelObjects; //存储世界的物体

		friend class Application;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] WorldName 世界的名字
		 * @param[in] Application 呈现世界的应用程序
		 */
		PixelWorld(std::string WorldName, Application* Application);

		/**
		 * @brief 析构函数
		 */
		~PixelWorld();

		/**
		 * @brief 设置使用的分辨率大小
		 * @param[in] width 分辨率的宽度
		 * @param[in] height 分辨率的高度
		 */
		void SetResolution(int width, int height);

		/**
		 * @brief 设置我们使用的摄像机
		 * @param[in] camera 摄像机
		 */
		void SetCamera(Camera* camera);

		/**
		 * @brief 设置我们使用的着色器
		 * @param[in] shader 着色器的指针，注意请保持它的生命周期
		 */
		void SetShader(Graphics::GraphicsShader* shader);

		/**
		 * @brief 设置我们使用的着色器为默认着色器，初始值就是默认着色器
		 */
		void SetShader();

		/**
		 * @brief 设置我们使用的世界地图，使用的地图必须要求注册
		 * @param[in] worldMapName 地图的名字
		 */
		void SetWorldMap(std::string worldMapName);

		/**
		 * @brief 设置我们使用的世界地图，如果地图没有注册那么我们将会为其注册
		 * @param[in] worldMap 要使用的世界地图
		 */
		void SetWorldMap(WorldMap* worldMap);

		/**
		 * @brief 设置渲染物体的大小
		 * @param[in] 渲染物体的大小
		 */
		void SetRenderObjectSize(float size);

		/**
		 * @brief 注册一个渲染物体，请注意保持纹理的生命周期
		 * @param[in] id 我们注册的渲染物体的ID，注意不能为0
		 * @param[in] fileData 渲染物体的数据信息，注意数据必须是数据格式R8G8B8A8
		 */
		void RegisterRenderObjectID(int id, Graphics::Texture2D* texture);

		/**
		 * @brief 释放我们注册过的渲染物体
		 * @param[in] id 我们要释放的渲染物体的ID
		 */
		void UnRegisterRenderObjectID(int id);

		/**
		 * @brief 注册世界的地图
		 * @param[in] worldMap 世界的地图
		 */
		void RegisterWorldMap(WorldMap* worldMap);

		/**
		 * @brief 注册世界的物体，被注册的物体将会加入到世界中去，如果物体之前在另外一个世界里，那么将会先从移除然后再加入
		 * @param[in] pixelObject 要被注册的物体
		 */
		void RegisterPixelObject(PixelObject* pixelObject);


		/**
		 * @brief 取消注册物体，物体将会从世界中移除
		 * @param[in] pixelObject 要被取消注册的物体
		 */
		void UnRegisterPixelObject(PixelObject* pixelObject);

		/**
		 * @brief 取消注册物体，物体将会从世界中移除
		 * @param[in] objectName 要被取消注册的物体的名字
		 */
		void UnRegisterPixelObject(std::string objectName);

		/**
		 * @brief 获取渲染物体的大小
		 * @return 渲染物体的大小
		 */
		auto GetRenderObjectSize() -> float;

		/**
		 * @brief 获取当前的世界地图
		 * @return 世界地图
		 */
		auto GetWorldMap() -> WorldMap*;

		/**
		 * @brief 获取在当前状况下的世界的图像
		 * @return 存储当前世界的图像的纹理的指针
		 */
		auto GetCurrentWorld() -> Graphics::Texture2D*;
	};
}
