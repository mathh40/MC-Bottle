#pragma once
#include <string>
#include "Chunk.h"

class IChunkProvider
{
public:
	virtual ~IChunkProvider() = default;
	virtual Chunk* getLoadedChunk(int32_t var1, int32_t var2) = 0;

	virtual Chunk* provideChunk(int32_t var1, int32_t var2) = 0;

	virtual bool tick() = 0;

	virtual std::string makeString() = 0;

	virtual bool isChunkGeneratedAt(int32_t var1, int32_t var2) = 0;
};
