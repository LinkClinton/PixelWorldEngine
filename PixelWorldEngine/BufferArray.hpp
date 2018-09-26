#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		* @brief 缓冲数组，即将缓冲数组以着色器资源的方式存储
		*/
		class BufferArray :public ShaderResource {
		protected:
			Graphics * graphics; //..

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