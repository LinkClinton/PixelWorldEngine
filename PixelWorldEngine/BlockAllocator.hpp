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
	 * @brief һ���ڴ����ͣ�������֧�ֵ��ڴ���С����
	 */
	struct Chunk {
		int BlockSize; //֧�ֵ��ڴ���С
		Block* Blocks; //�ڴ��
	};

	/**
	 * @brief �ڴ�飬�乹��Ϊһ������
	 */
	struct Block {
		Block* nextBlock; //��һ��Ԫ��
	};

	/**
	 * @brief �ڴ�����
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
	 * @brief �ڴ������
	 */
	class BlockAllocator {
	private:
		Chunk* chunks; //�ڴ��������飬���ڹ����ڴ�
		
		int chunkCount; //ʹ�õ�����
		int chunkArraySize; //�����С

		Block* freeBlock[BLOCK_TYPE_COUNT]; //�������飬ÿ������洢��ͬ���͵Ŀ飬���еĿ鶼��δʹ�õĿɷ����

		static int blockSizes[BLOCK_TYPE_COUNT]; //��ȡ��ͬ��Щ������Ĵ�С
		static int blockSizeIndex[MAX_BLOCK_SIZE + 1]; //ӳ�䲻ͬ��С��Χ�����ڵ��ڴ������
		static bool isInitialized; //�Ƿ񽫾�̬������ʼ����
	public:
		/**
		 * @brief ���캯��
		 */
		BlockAllocator();

		/**
		 * @brief ��������
		 */
		~BlockAllocator();

		/**
		 * @brief �����ڴ�
		 * @param[in] size �ڴ��С����λbyte
		 * @return �����ڴ���ַ
		 */
		auto Allocate(int size) -> void*;

		/**
		* @brief �����ڴ棬ģ�巽ʽ
		* @return �����ڴ���ַ
		*/
		template<typename T>
		auto Allocate() -> T*;

		/**
		 * @brief �ͷ��ڴ�
		 * @param[in] pointer �ڴ��ָ���ַ
		 * @param[in] size �ڴ��С����λbyte
		 */
		void Free(void* pointer, int size);

		/**
		* @brief �ͷ��ڴ棬ģ�巽ʽ
		* @param[in] pointer �ڴ��ָ���ַ
		*/
		template<typename T>
		void Free(T* pointer);

		/**
		 * @brief ���������ڴ�
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
