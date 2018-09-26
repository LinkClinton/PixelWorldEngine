#include "BlockAllocator.hpp"
#include "DebugLayer.hpp"

#include <memory>

int PixelWorldEngine::BlockAllocator::blockSizes[BLOCK_TYPE_COUNT];
int PixelWorldEngine::BlockAllocator::blockSizeIndex[MAX_BLOCK_SIZE + 1];
bool PixelWorldEngine::BlockAllocator::isInitialized = false;

PixelWorldEngine::BlockAllocator::BlockAllocator()
{
	chunkArraySize = CHUNK_ARRAY_ADD; //��ʼ�����С
	chunkCount = 0; //ʹ�ô�СΪ0
	chunks = (Chunk*)std::malloc(chunkArraySize * sizeof(Chunk)); //�����ڴ�ռ䣬���������ڷ����

	memset(chunks, 0, chunkArraySize * sizeof(Chunk)); //����
	memset(freeBlock, 0, sizeof(freeBlock));

	if (isInitialized == true) return; //���þ�̬����

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
		std::free(chunks[i].Blocks); //�ͷŷ�����ڴ�ռ�

	std::free(chunks); //�ͷ�����ʹ�õ��ڴ�ռ�
}

auto PixelWorldEngine::BlockAllocator::Allocate(int size) -> void *
{
	if (size == 0) return nullptr; //Ϊ0���򲻷���

	if (size > MAX_BLOCK_SIZE) return std::malloc(size); //����Ŀռ�̫������ֱ�ӷ���

	DebugThrow(DebugLayer::Assert(size < 0, Error::MemoryAllocateError, "size is less than 0.")); //С��0�򱨴�

	int index = blockSizeIndex[size]; //��ȡ������

	if (freeBlock[index] != nullptr) { //�����δ����Ŀ飬��ôֱ�ӷ���

		auto result = freeBlock[index];
		freeBlock[index] = freeBlock[index]->nextBlock;

		return result;
	}

	if (chunkCount == chunkArraySize) { //����chunk��С

		auto oldChunk = chunks;

		chunkArraySize += CHUNK_ARRAY_ADD;

		chunks = (Chunk*)std::malloc(chunkArraySize * sizeof(Chunk));

		memcpy(chunks, oldChunk, chunkCount * sizeof(Chunk));
		memset(chunks + chunkCount, 0, CHUNK_ARRAY_ADD * sizeof(Chunk));
		
		std::free(oldChunk);
	}

	auto chunk = chunks + chunkCount; //��ǰʹ�õ�chunk

	chunk->Blocks = (Block*)std::malloc(CHUNK_SIZE); //���������

#ifdef _DEBUG
	memset(chunk->Blocks, 0, CHUNK_SIZE);
#endif // _DEBUG


	chunk->BlockSize = blockSizes[index];
	
	int blockCount = CHUNK_SIZE / chunk->BlockSize; //�������

	for (int i = 0; i < blockCount; i++) { //�����������
		auto block = (Block*)((byte*)chunk->Blocks + chunk->BlockSize * i);
		block->nextBlock = (Block*)((byte*)chunk->Blocks + chunk->BlockSize * (i + 1));
	}

	auto last = (Block*)((byte*)chunk->Blocks + chunk->BlockSize * (blockCount - 1));
	last->nextBlock = nullptr;

	freeBlock[index] = chunk->Blocks->nextBlock; //����δ�����

	chunkCount++;

	return chunk->Blocks; //���ط�����ڴ�ռ�
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



