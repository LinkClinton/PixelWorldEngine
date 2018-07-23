#pragma once

#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		* @brief 二维纹理数组
		*/
		class Texture2DArray :public ShaderResource {
		private:
			Graphics * graphics; //..

			int width, height; //宽度和高度

			int size; //大小，单位byte
			int rowPitch; //一行元素的大小
			int mipLevels; //mipLevel
			int arraySize; //数组大小

			PixelFormat pixelFormat; //像素格式

#ifdef _WIN32
		public:
			D3D11_TEXTURE2D_DESC desc; //描述纹理接口
#endif // _WIN32

		public:
			/**
			* @brief 构造函数
			* @param[in] graphics 图形接口
			* @param[in] Width 宽度
			* @param[in] Height 高度
			* @param[in] arraySize 数组大小
			* @param[in] PixelFormat 像素格式
			* @param[in] MipLevels mipLevel
			*/
			Texture2DArray(Graphics* graphics, int Width, int Height, int arraySize = 1, PixelFormat PixelFormat = PixelFormat::Unknown, int MipLevels = 1);

			/**
			* @brief 析构函数
			*/
			~Texture2DArray();

			/**
			* @brief 将其他纹理的一部分复制过来
			* @parma[in] srcTexture 被复制的纹理
			* @param[in] dstPositionX 复制到的区域的X起始位置
			* @param[in] dstPositionY 复制到的区域的Y起始位置
			* @param[in] arrayIndex 要复制到的纹理索引
			* @param[in] srcRect 要复制的源纹理的区域
			*/
			void CopyFromTexture2D(Texture2D* srcTexture, int dstPositionX, int dstPositionY, int arrayIndex, PixelWorldEngine::Rectangle srcRect);

			/**
			 * @brief 将纹理数组复制到过来
			 * @param[in] srcTextureArray 要被复制的纹理数组
			 * @param[in] dstArrayStartIndex 复制到的数组的起始位置
			 * @param[in] srcArrayStartIndex 被复制的数组的起始位置
			 * @param[in] arraySize 要复制的大小
			 */
			void CopyFromTexture2DArray(Texture2DArray* srcTextureArray, int dstArrayStartIndex, int srcArrayStartIndex, int arraySize);

			/**
			* @brief 更新数据
			* @param[in] data 数据，注意大小一致
			* @param[in] arrayIndex 要复制到的纹理索引
			*/
			void Update(void* data, int arrayIndex);

			/**
			* @brief 获取宽度
			* @return 宽度
			*/
			auto GetWidth() -> int;

			/**
			* @brief 获取高度
			* @return 高度
			*/
			auto GetHeight() -> int;

			/**
			* @brief 获取纹理大小
			* @return 纹理大小，单位byte
			*/
			auto GetSize() -> int;

			/**
			 * @brief 返回纹理数组大小
			 * @return 纹理数组大小
			 */
			auto GetArraySize() -> int;

			/**
			* @brief 获取MipLevel
			* @return MipLevel11
			*/
			auto GetMipLevels() -> int;

			/**
			* @brief 获取像素格式
			* @return 像素格式
			*/
			auto GetPixelFormat()->PixelFormat;
		};


	}
}