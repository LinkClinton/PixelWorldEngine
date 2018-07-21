#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

#define BLOCK_TYPE_COUNT 14
#define MAX_BLOCK_SIZE 640

#define CHUNK_SIZE 32768
#define CHUNK_ARRAY_ADD 128

	struct Chunk;
	struct Block;

	/**
	 * @brief 一种内存类型，按照其支持的内存块大小区分
	 */
	struct Chunk {
		int BlockSize; //支持的内存块大小
		Block* Blocks; //内存块
	};

	/**
	 * @brief 内存块，其构成为一个链表
	 */
	struct Block {
		Block* nextBlock; //下一个元素
	};

	/**
	 * @brief 内存类型
	 */
	enum ChunkType {
		Byte16, //16byte
		Byte32, //32byte
		Byte64, //64byte
		Byte96, //96byte
		Byte128, //128byte
		Byte160, //160byte
		Byte192, //192byte
		Byte224, //224byte
		Byte256, //256byte
		Byte320, //320byte
		Byte384, //384byte
		Byte448, //448byte
		Byte512, //512byte
		Byte640 //640byte
	};

	/**
	 * @brief 内存分配器
	 */
	class BlockAllocator {
	private:
		Chunk* chunks; //内存类型数组，用于管理内存
		
		int chunkCount; //使用的数量
		int chunkArraySize; //数组大小

		Block* freeBlock[BLOCK_TYPE_COUNT]; //链表数组，每个链表存储不同类型的块，所有的块都是未使用的可分配块

		static int blockSizes[BLOCK_TYPE_COUNT]; //获取不同了些的数组的大小
		static int blockSizeIndex[MAX_BLOCK_SIZE + 1]; //映射不同大小范围其属于的内存块类型
		static bool isInitialized; //是否将静态变量初始化了
	public:
		/**
		 * @brief 构造函数
		 */
		BlockAllocator();

		/**
		 * @brief 析构函数
		 */
		~BlockAllocator();

		/**
		 * @brief 分配内存
		 * @param[in] size 内存大小，单位byte
		 * @return 返回内存块地址
		 */
		auto Allocate(int size) -> void*;

		/**
		* @brief 分配内存，模板方式
		* @return 返回内存块地址
		*/
		template<typename T>
		auto Allocate() -> T*;

		/**
		 * @brief 释放内存
		 * @param[in] pointer 内存块指针地址
		 * @param[in] size 内存大小，单位byte
		 */
		void Free(void* pointer, int size);

		/**
		* @brief 释放内存，模板方式
		* @param[in] pointer 内存块指针地址
		*/
		template<typename T>
		void Free(T* pointer);

		/**
		 * @brief 清理所有内存
		 */
		void Clear();
	};

	template<typename T>
	inline auto BlockAllocator::Allocate() -> T *
	{
		return (T*)Allocate(sizeof(T));
	}

	template<typename T>
	inline void BlockAllocator::Free(T * pointer)
	{
		Free(pointer, sizeof(T));
	}

}
