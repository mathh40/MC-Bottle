#pragma once
#include "WorldServer.h"
#include "chunk/IChunkProvider.h"

class ChunkProviderServer : public IChunkProvider
{
  public:
    ChunkProviderServer(WorldServer *worldObjIn, IChunkLoader *chunkLoaderIn, IChunkGenerator *chunkGeneratorIn);
    std::unordered_map<int64_t, Chunk *> &getLoadedChunks();
    void queueUnload(Chunk *chunkIn);
    void queueUnloadAll();
    Chunk *getLoadedChunk(int32_t x, int32_t z) override;
    Chunk *loadChunk(int32_t x, int32_t z);
    Chunk *provideChunk(int32_t x, int32_t z) override;
    bool saveChunks(bool all);
    void flushToDisk();
    bool tick() override;
    bool canSave() const;
    std::string makeString() override;
    std::vector<SpawnListEntry> getPossibleCreatures(EnumCreatureType creatureType, BlockPos &pos);
    std::optional<BlockPos> getNearestStructurePos(World *worldIn, std::string_view structureName, BlockPos &position,
                                                   bool findUnexplored);
    bool isInsideStructure(World *worldIn, std::string_view structureName, BlockPos &pos) const;
    int32_t getLoadedChunkCount() const;
    bool chunkExists(int32_t x, int32_t z);
    bool isChunkGeneratedAt(int32_t x, int32_t z) override;

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::unordered_set<int64_t> droppedChunks;
    IChunkGenerator *chunkGenerator;
    IChunkLoader *chunkLoader;
    std::unordered_map<int64_t, Chunk *> loadedChunks;
    WorldServer *world;

    Chunk *loadChunkFromFile(int32_t x, int32_t z);
    void saveChunkExtraData(Chunk *chunkIn);
    void saveChunkData(Chunk *chunkIn);
};
