#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief 着色器资源
		 */
		class ShaderResource {
#ifdef _WIN32
		public:
			ID3D11Resource* resource; //Direct3D 接口
			ID3D11ShaderResourceView* resourceView; //Direct3D 接口
#endif // _WIN32
		public:
			/**
			 * @brief 析构函数
			 */
			~ShaderResource();

		};

		/**
		 * @brief 二维纹理
		 */
		class Texture2D :public ShaderResource {
		protected:
			Graphics* graphics; //..

			int width, height; //宽度和高度

			int size; //大小，单位byte
			int rowPitch; //一行元素的大小
			int mipLevels; //mipLevel

			PixelFormat pixelFormat; //像素格式

#ifdef _WIN32
		public:
			D3D11_TEXTURE2D_DESC desc; //描述纹理接口
#endif // _WIN32

		public:
			/**
			 * @brief 构造函数
			 * @param[in] graphics 图形接口
			 * @param[in] Data 数据
			 * @param[in] Width 宽度
			 * @param[in] Height 高度
			 * @param[in] PixelFormat 像素格式
			 * @param[in] MipLevels mipLevel
			 */
			Texture2D(Graphics* graphics, void* Data, int Width, int Height, PixelFormat PixelFormat = PixelFormat::Unknown, int MipLevels = 1);

			/**
			 * @brief 构造函数，从其他纹理构造出子纹理
			 * @param[in] srcTexture 源纹理
			 * @param[in] srcRect 要构造的子纹理范围
			 */
			Texture2D(Texture2D* srcTexture, PixelWorldEngine::Rectangle srcRect);

			/**
			 * @brief 析构函数
			 */
			~Texture2D();

			/**
			 * @brief 将其他纹理的一部分复制过来
			 * @parma[in] srcTexture 被复制的纹理
			 * @param[in] 复制到的区域的X起始位置
			 * @param[in] 复制到的区域的Y起始位置
			 * @param[in] srcRect 要复制的源纹理的区域
			 */
			void CopyFromTexture2D(Texture2D* srcTexture, int dstPositionX, int dstPositionY, PixelWorldEngine::Rectangle srcRect);

			/**
			 * @brief 更新数据
			 * @param[in] data 数据，注意大小一致
			 */
			void Update(void* data);

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

		/**
		 * @brief 缓冲数组，即将缓冲数组以着色器资源的方式存储
		 */
		class BufferArray :public ShaderResource {
		protected:
			Graphics* graphics; //..

			int size; //大小，单位byte
			int count; //数组的长度

#ifdef _WIN32
		public:
			D3D11_BUFFER_DESC desc; //描述缓冲结构

			ID3D11Buffer* buffer; //Direct3D 接口
#endif // _WIN32
			
		public:
			/**
			 * @brief 构造函数
			 * @param[in] graphics 图形接口
			 * @param[in] data 数据信息
			 * @param[in] dataSize 数据大小
			 * @param[in] dataCount 数据如果是数组，那么其长度，否则则设置为1
			 */
			BufferArray(Graphics* graphics, void* data, int dataSize, int dataCount = 1);

			/**
			 * @brief 析构函数
			 */
			~BufferArray();

			/**
			 * @brief 更新数据
			 * @param[in] 数据，注意数据大小一致
			 */
			void Update(void* data);

			/**
			 * @brief 获取缓冲大小
			 * @return 缓冲大小，单位byte
			 */
			auto GetSize() -> int;

			/**
			 * @brief 获取数量
			 * @return 数量
			 */
			auto GetCount() -> int;
		};
	}
}
