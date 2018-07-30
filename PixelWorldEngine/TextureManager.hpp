#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"
#include "WorldMap.hpp"

#define TEXTURE_WIDTH 2048
#define TEXTURE_HEIGHT 2048
#define TEXTURE_ARRAY_ADD 8
#define MAX_TEXTURE_SIZE 2048

namespace PixelWorldEngine {

	/**
	 * @brief 纹理类型
	 */
	enum TextureType {
		Size128 = 128, //宽度和高度大小为128
		Size256 = 256, //宽度和高度大小为256
		Size512 = 512, //宽度和高度大小为512
		Size1024 = 1024, //宽度和高度大小为1024
		Size2048 = 2048, //宽度和高度大小为2048
		Count = 5
	};

	/**
	 * @brief 子纹理
	 */
	struct SubTexture {
		int arrayIndex; //纹理所在的数组

		int startPositionX; //纹理的起始X位置
		int startPositionY; //纹理的起始Y位置
		int width; //纹理的宽度
		int height; //纹理的高度

		glm::mat4x4 transform; //纹理坐标的变换矩阵

		void MakeTexCoordPosition(int maxWidth, int maxHeight);
	};

	/**
	 * @brief 纹理管理器
	 */
	class TextureManager {
	private:
		Graphics::Graphics* graphics; //..

		Graphics::Texture2DArray* textureArray; //存储纹理的纹理数组

		Graphics::PixelFormat pixelFormat; //纹理格式

		std::map<int, SubTexture> subTextures; //子纹理

		std::queue<SubTexture> freeSubTextures[(int)TextureType::Count]; //空闲的子纹理

		int textureCount; //纹理使用量
		int textureSpace; //纹理总共量

		static int textureSizes[(int)TextureType::Count]; //纹理支持的大小
		static int textureSizeIndex[MAX_TEXTURE_SIZE + 1]; //不同大小的纹理对应的纹理类型
		static bool isInitialized; //是否初始化

		/**
		 * @brief 将子纹理复制到纹理数组中去
		 * @param[in] id 子纹理的渲染编号
		 * @param[in] subTexture 子纹理
		 * @param[in] texture 子纹理数据
		 */
		void CopySubTexture(int id, SubTexture subTexture, Graphics::Texture2D* texture);
	public:
		/**
		 * @brief 构造函数
		 * @param[in] application 应用程序
		 * @param[in] pixelFormat 纹理格式
		 */
		TextureManager(Application* application, Graphics::PixelFormat pixelFormat = Graphics::PixelFormat::Unknown);

		/** 
		 * @brief 析构函数
		 */
		~TextureManager();

		/**
		 * @brief 注册纹理
		 * @param[in] id 渲染编号
		 * @param[in] texture 纹理
		 */
		void RegisterTexture(int id, Graphics::Texture2D* texture);

		/**
		 * @brief 取消注册纹理
		 * @param[in] id 渲染编号
		 */
		void UnRegisterTexture(int id);

		/**
		 * @brief 清空
		 */
		void Clear();

		/**
		 * @brief 通过渲染编号获取纹理变换
		 * @param[in] id 渲染编号
		 * @return 纹理变换
		 */
		auto GetTexCoordTransform(int id) -> glm::mat4x4;

		/**
		 * @brief 获取纹理数组
		 * @return 纹理数组
		 */
		auto GetTextureArray() -> Graphics::Texture2DArray*;

		/**
		 * @brief 获取对应渲染编号所在的纹理数组索引
		 * @return 数组索引
		 */
		auto GetArrayIndex(int id) -> int;

		/**
		 * @brief 检测这个渲染编号是否存在
		 * @return 返回值为true代表存在，否则不存在
		 */
		auto IsExist(int id) -> bool;
	};
	
}