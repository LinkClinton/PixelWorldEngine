#include "BlockAllocator.hpp"
#include "DebugLayer.hpp"

#include <memory>

int PixelWorldEngine::BlockAllocator::blockSizes[BLOCK_TYPE_COUNT];
int PixelWorldEngine::BlockAllocator::blockSizeIndex[MAX_BLOCK_SIZE + 1];
bool PixelWorldEngine::BlockAllocator::isInitialized = false;

PixelWorldEngine::BlockAllocator::BlockAllocator()
{
	chunkArraySize = CHUNK_ARRAY_ADD; //初始数组大小
	chunkCount = 0; //使用大小为0
	chunks = (Chunk*)std::malloc(chunkArraySize * sizeof(Chunk)); //创建内存空间，但不是用于分配的

	memset(chunks, 0, chunkArraySize * sizeof(Chunk)); //清零
	memset(freeBlock, 0, sizeof(freeBlock));

	if (isInitialized == true) return; //设置静态变量

	blockSizes[0] = 16;
	blockSizes[1] = 32;

	for (int i = 2; i <= 8; i++)
		blockSizes[i] = blockSizes[i - 1] + 32;

	for (int i = 9; i <= 12; i++)
		blockSizes[i] = blockSizes[i - 1] + 64;

	for (int i = 13; i < BLOCK_TYPE_COUNT; i++)
		blockSizes[i] = blockSizes[i - 1] + 128;

	int index = 0;

	for (int i = 1; i <= MAX_BLOCK_SIZE; i++) {
		blockSizeIndex[i] = index;

		if (i == blockSizes[index]) index++;
	}

	isInitialized = true;
}

PixelWorldEngine::BlockAllocator::~BlockAllocator()
{
	for (int i = 0; i < chunkCount; i++)
		std::free(chunks[i].Blocks); //释放分配的内存空间

	std::free(chunks); //释放内置使用的内存空间
}

auto PixelWorldEngine::BlockAllocator::Allocate(int size) -> void *
{
	if (size == 0) return nullptr; //为0，则不分配

	if (size > MAX_BLOCK_SIZE) return std::malloc(size); //分配的空间太大，我们直接分配

	DebugThrow(DebugLayer::Assert(size < 0, Error::MemoryAllocateError, "size is less than 0.")); //小于0则报错

	int index = blockSizeIndex[size]; //获取块类型

	if (freeBlock[index] != nullptr) { //如果有未分配的块，那么直接分配

		auto result = freeBlock[index];
		freeBlock[index] = freeBlock[index]->nextBlock;

		return result;
	}

	if (chunkCount == chunkArraySize) { //增加chunk大小

		auto oldChunk = chunks;

		chunkArraySize += CHUNK_ARRAY_ADD;

		chunks = (Chunk*)std::malloc(chunkArraySize * sizeof(Chunk));

		memcpy(chunks, oldChunk, chunkCount * sizeof(Chunk));
		memset(chunks + chunkCount, 0, CHUNK_ARRAY_ADD * sizeof(Chunk));
		
		std::free(oldChunk);
	}

	auto chunk = chunks + chunkCount; //当前使用的chunk

	chunk->Blocks = (Block*)std::malloc(CHUNK_SIZE); //分配块数据

#ifdef _DEBUG
	memset(chunk->Blocks, 0, CHUNK_SIZE);
#endif // _DEBUG


	chunk->BlockSize = blockSizes[index];
	
	int blockCount = CHUNK_SIZE / chunk->BlockSize; //块的数量

	for (int i = 0; i < blockCount; i++) { //构建块的链表
		auto block = (Block*)((byte*)chunk->Blocks + chunk->BlockSize * i);
		block->nextBlock = (Block*)((byte*)chunk->Blocks + chunk->BlockSize * (i + 1));
	}

	auto last = (Block*)((byte*)chunk->Blocks + chunk->BlockSize * (blockCount - 1));
	last->nextBlock = nullptr;

	freeBlock[index] = chunk->Blocks->nextBlock; //设置未分配块

	chunkCount++;

	return chunk->Blocks; //返回分配的内存空间
}

void PixelWorldEngine::BlockAllocator::Free(void * pointer, int size)
{
	if (size == 0) return;

	if (size > MAX_BLOCK_SIZE) return std::free(pointer);

	DebugThrow(DebugLayer::Assert(size < 0, Error::MemoryAllocateError, "size is less than 0."));

	int index = blockSizeIndex[size];

	auto block = (Block*)pointer;
	block->nextBlock = freeBlock[index];
	freeBlock[index] = block;
}

void PixelWorldEngine::BlockAllocator::Clear()
{
	for (int i = 0; i < chunkCount; i++)
		std::free(chunks[i].Blocks);

	chunkCount = 0;

	memset(chunks, 0, chunkArraySize * sizeof(Chunk));
	memset(freeBlock, 0, sizeof(freeBlock));
}



