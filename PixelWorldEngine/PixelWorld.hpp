#pragma once

#include "pch.hpp"

#include "TextureManager.hpp"
#include "DataManager.hpp"
#include "Graphics.hpp"
#include "Events.hpp"

#include "WorldMap.hpp"
#include "PixelObject.hpp"
#include "Geometry.hpp"
#include "Utility.hpp"
#include "Camera.hpp"

#include "UIObject.hpp"

namespace PixelWorldEngine {

#define LOW_MAX_INSTANCE_DATA 100

	/**
	 * @brief 用来表述用途不同的缓冲的在着色器以及数组的编号
	 */
	enum class BufferIndex : int {
		CameraBuffer, //摄像机矩阵缓冲
		RenderConfig, //记录当前渲染的时候的设置
		LowInstanceData, //用于低版本的实例技术
		Count
	};

	/**
	 * @brief 用来表述不同用途的缓冲数组的索引
	 */
	enum class BufferArrayIndex :int {
		WorldMapInstanceData, //渲染地图实例数据
		PixelObjectInstanceData, //渲染PixelObject实例数据
		UIObjectInstanceData, //渲染UIObject实例数据
		Count
	};

	/**
	 * @brief 全局渲染设置，目前不使用
	 */
	struct PixelWorldRenderConfig {
		Graphics::PixelFormat mergeTextureFormat[MAX_MERGETEXTURE_COUNT]; //使用的合并纹理的格式
		glm::vec4 unused[3];
	};

	 /**
	 * @brief 实例数据
	 */
	struct InstanceData {
		int setting[4]; //设置，第一个元素是使用的渲染编号，第二个元素是使用的纹理页编号
		glm::mat4x4 worldTransform; //世界变换矩阵
		glm::mat4x4 texcoordTransform; //纹理变换矩阵
		glm::vec4 renderCoor; //渲染的颜色，第四个分量是不透明度

		InstanceData();
	};

	/**
	 * @brief 引擎的核心世界
	 */
	class PixelWorld {
	private:
		std::string worldName; //世界的名字 

		int resolutionWidth; //分辨率宽度
		int resolutionHeight; //分辨率高度

		float ssaaLevel = 1.0f; //SSAA等级

		PixelWorldRenderConfig renderConfig; //渲染设置数据

		Camera* camera; //摄像机
		Camera UICamera; //UI摄像机

		float backGroundColor[4]; //背景颜色，默认为(0, 0, 0, 1)

		Graphics::Graphics* graphics; //...

		Graphics::Texture2D* renderBuffer; //存储世界被渲染后的像素信息
		Graphics::RenderTarget* renderTarget; //..

		Graphics::GraphicsShader* defaultShader; //默认着色器
		Graphics::GraphicsShader* shader; //使用的着色器

		std::vector<Graphics::Buffer*> buffers; //缓冲数组
		std::vector<Graphics::BufferArray*> bufferArrays; //缓冲数组

		Graphics::RectangleF* renderObject; //正方形
		Graphics::RectangleF* renderCanvas; //画布

		Graphics::StaticSampler* defaultSampler; //默认的采样器

		TextureManager* textureManager; //纹理管理

		WorldMap* worldMap; //当前使用的地图，默认为nullptr

		std::map<std::string, WorldMap*> worldMaps; //存储世界的地图
		std::map<std::string, PixelObject*> pixelObjects; //存储世界的物体
		std::map<std::string, UIObject*> UIObjects; //存储UI物体
		
		std::multiset<PixelObject*, PixelObjectCompare> pixelObjectLayer;//存储不同层的PixelObject
		std::multiset<UIObject*, UIObjectCompare> UIObjectLayer; //存储不同层的UIObject

		UIObject* focusUIObject; //得到焦点的UIObject，默认为nullptr

		friend class UIObject;
		friend class PixelObject;
		friend class Application;
	private:
		/**
		* @brief 当鼠标移动的时候触发
		* @param[in] sender 谁触发了消息
		* @param[in] eventArg 消息信息
		*/
		void OnMouseMove(void* sender, Events::MouseMoveEvent* eventArg);

		/**
		* @brief 当鼠标按下的时候触发
		* @param[in] sender 谁触发了消息
		* @param[in] eventArg 消息信息
		*/
		void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg);

		/**
		* @brief 当鼠标滑轮滚动的时候触发
		* @param[in] sender 谁触发了消息
		* @param[in] eventArg 消息信息
		*/
		void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg);

		/**
		* @brief 当键盘按键按下去的时候触发
		* @param[in] sender 谁触发了消息
		* @param[in] eventArg 消息信息
		*/
		void OnKeyClick(void* sender, Events::KeyClickEvent* eventArg);

		/**
		 * @brief 更新世界，当应用程序更新的时候，世界也会更新
		 * @param[in] deltaTime 距离上一次更新的时间
		 */
		void OnUpdate(float deltaTime);

		/**
		 * @brief 用于低版本的渲染
		 * @param[in] instanceData 实例数据
		 */
		void LowDrawObject(std::vector<InstanceData>* instanceData);

		/**
		 * @brief 用于高版本的渲染
		 * @param[in] instanceData 实例数据
		 * @param[in] arrayIndex 索引
		 */
		void HighDrawObject(std::vector<InstanceData>* instanceData, BufferArrayIndex arrayIndex);

		/**
		 * @brief 渲染世界的地图，作为GetCurrentWorld的子部分
		 */
		void RenderWorldMap();

		/**
		 * @brief 渲染像素物体，作为GetCurrentWorld的子部分
		 */
		void RenderPixelObjects();

		/**
		 * @brief 渲染UI物体
		 * @param[in] baseTransform 父亲的位移
		 * @param[in] baseOpacity 父亲的不透明度
		 * @param[in] object 物体
		 * @param[in] instanceData 构建的实例数据
		 */
		void RenderUIObject(glm::mat4x4 baseTransform, float baseOpacity, UIObject* object, std::vector<InstanceData>* instanceData);

		/**
		 * @brief 渲染UI物体，作为GetCurrentWorld的子部分
		 */
		void RenderUIObjects();
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
		 * @param[in] ssaa 设置缓冲倍数，用于抗锯齿，默认为1.0f
		 */
		void SetResolution(int width, int height, float ssaa = 1.0f);

		/**
		 * @brief 设置背景颜色
		 * @param[in] red 分量，默认为0
		 * @param[in] green 分量，默认为0
		 * @param[in] blue 分量，默认为0
		 * @param[in] alpha 分量，默认为1
		 */
		void SetBackGroundColor(float red, float green, float blue, float alpha);

		/**
		 * @brief 设置我们使用的摄像机
		 * @param[in] camera 摄像机，默认为nullptr
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
		 * @brief 设置纹理管理器，用于管理不同的渲染ID使用的纹理
		 * @param[in] textureManager 纹理管理器
		 */
		void SetTextureManager(TextureManager* textureManager);

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
		 * @brief 注册UI物体，被注册的物体将会加入到世界中去，如果物体之前在另外一个世界里，那么将会先从移除然后再加入
		 * @param[in] object 物体
		 */
		void RegisterUIObject(UIObject* object);

		/**
		 * @brief 取消注册物体
		 * @param[in] object 物体
		 */
		void UnRegisterUIObject(UIObject* object);

		/**
		 * @brief 取消注册物体
		 * @param[in] object 物体的名字
		 */
		void UnRegisterUIObject(std::string name);

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
