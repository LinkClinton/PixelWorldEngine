#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief 缓冲的类型
		 */
		enum class BufferType {
			VertexBuffer = 1, //顶点缓冲，用于存储顶点数据
			IndexBuffer = 2, //索引缓冲，用于存储顶点索引数据
			ConstantBuffer = 4 //常量缓冲，用于存储提交给着色器的数据
		};

		/**
		 * @brief 缓冲类型
		 */
		class Buffer {
		private:
			Graphics* graphics; //..

			int size; //缓冲的大小，单位byte
			int count; //默认为1

			BufferType type; //缓冲的类型

#ifdef _WIN32
		public:
			D3D11_BUFFER_DESC desc; //描述缓冲的结构

			ID3D11Buffer* buffer; //缓冲资源
#endif // _WIN32

		public:
			/**
			 * @brief 构造函数
			 * @param[in] data 要放入缓冲中的数据
			 * @param[in] dataSize 缓冲的大小，单位byte
			 * @param[in] dataCount 元素个数，放入缓冲中的如果是一个数组，那么请将其设置为数组的长度否则设置为1
			 * @param[in] bufferType 缓冲的类型
			 */
			Buffer(Graphics* graphics, void* data, int dataSize, int dataCount = 1, BufferType bufferType = BufferType::ConstantBuffer);
			
			/**
			 * @brief 析构函数
			 */
			~Buffer();

			/**
			 * @brief 将数据更新到缓冲中去
			 * @param[in] data 数据，请保证数据大小和缓冲大小一致
			 */
			void Update(void* data);

			/** 
			 * @brief 获取缓冲大小
			 * @return 缓冲大小，单位byte
			 */
			auto GetSize() -> int;

			/**
			 * @brief 返回缓冲中的元素个数
			 * @return 元素个数
			 */
			auto GetCount() -> int;

			/**
			 * @brief 返回缓冲的类型
			 * @return 缓冲的类型
			 */
			auto GetType() -> BufferType;
		};
	}
}
