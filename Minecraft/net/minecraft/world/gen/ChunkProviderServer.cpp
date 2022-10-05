#include "ChunkProviderServer.h"

#include "MinecraftException.h"
#include "ReportedException.h"

std::shared_ptr<spdlog::logger> ChunkProviderServer::LOGGER = spdlog::get("Minecraft")->clone("ChunkProviderServer");

ChunkProviderServer::ChunkProviderServer(WorldServer *worldObjIn, IChunkLoader *chunkLoaderIn,
                                         IChunkGenerator *chunkGeneratorIn)
    : world(worldObjIn), chunkLoader(chunkLoaderIn), chunkGenerator(chunkGeneratorIn)
{
    loadedChunks.reserve(8192);
}

std::unordered_map<int64_t, Chunk *> &ChunkProviderServer::getLoadedChunks()
{
    return loadedChunks;
}

void ChunkProviderServer::queueUnload(Chunk *chunkIn)
{
    if (world->provider->canDropChunk(chunkIn->x, chunkIn->z))
    {
        droppedChunks.emplace(ChunkPos::asLong(chunkIn->x, chunkIn->z));
        chunkIn->unloadQueued = true;
    }
}

void ChunkProviderServer::queueUnloadAll()
{
    for (auto &chunk : loadedChunks)
    {
        queueUnload(chunk);
    }
}

Chunk *ChunkProviderServer::getLoadedChunk(int32_t x, int32_t z)
{
    auto i     = ChunkPos::asLong(x, z);
    auto chunk = loadedChunks.at(i);
    if (chunk != nullptr)
    {
        chunk->unloadQueued = false;
    }

    return chunk;
}

Chunk *ChunkProviderServer::loadChunk(int32_t x, int32_t z)
{
    auto chunk = getLoadedChunk(x, z);
    if (chunk == nullptr)
    {
        chunk = loadChunkFromFile(x, z);
        if (chunk != nullptr)
        {
            loadedChunks.emplace(ChunkPos::asLong(x, z), chunk);
            chunk->onLoad();
            chunk->populate(this, chunkGenerator);
        }
    }

    return chunk;
}

Chunk *ChunkProviderServer::provideChunk(int32_t x, int32_t z)
{
    auto chunk = loadChunk(x, z);
    if (chunk == nullptr)
    {
        long i = ChunkPos::asLong(x, z);

        try
        {
            chunk = chunkGenerator->generateChunk(x, z);
        }
        catch (Throwable var9)
        {
            CrashReport crashreport = CrashReport.makeCrashReport(var9, "Exception generating new chunk");
            CrashReportCategory crashreportcategory = crashreport.makeCategory("Chunk to be generated");
            crashreportcategory.addCrashSection("Location", String.format("%d,%d", x, z));
            crashreportcategory.addCrashSection("Position hash", i);
            crashreportcategory.addCrashSection("Generator", chunkGenerator);
            throw ReportedException(crashreport);
        }

        loadedChunks.emplace(i, chunk);
        chunk->onLoad();
        chunk->populate(this, chunkGenerator);
    }

    return chunk;
}

bool ChunkProviderServer::saveChunks(bool all)
{
    auto i = 0;

    for (auto &list : droppedChunks)
    {
        auto chunk = list->second();
        if (all)
        {
            saveChunkExtraData(chunk);
        }

        if (chunk->needsSaving(all))
        {
            saveChunkData(chunk);
            chunk->setModified(false);
            ++i;
            if (i == 24 && !all)
            {
                return false;
            }
        }
    }

    return true;
}

void ChunkProviderServer::flushToDisk()
{
    chunkLoader->flush();
}

bool ChunkProviderServer::tick()
{
    if (!world->disableLevelSaving)
    {
        if (!droppedChunks.empty())
        {
            auto iterator = droppedChunks.begin();

            for (auto i = 0; i < 100 && iterator != droppedChunks.end();)
            {
                auto olong = *iterator;
                auto chunk = loadedChunks.at(olong);
                if (chunk != nullptr && chunk->unloadQueued)
                {
                    chunk->onUnload();
                    saveChunkData(chunk);
                    saveChunkExtraData(chunk);
                    loadedChunks.erase(olong);
                    ++i;
                    iterator = droppedChunks.erase(iterator);
                }
            }
        }

        chunkLoader->chunkTick();
    }

    return false;
}

bool ChunkProviderServer::canSave() const
{
    return !world->disableLevelSaving;
}

std::vector<SpawnListEntry> ChunkProviderServer::getPossibleCreatures(EnumCreatureType creatureType, BlockPos &pos)
{
    return chunkGenerator->getPossibleCreatures(creatureType, pos);
}

std::optional<BlockPos> ChunkProviderServer::getNearestStructurePos(World *worldIn, std::string_view structureName,
                                                                    BlockPos &position, bool findUnexplored)
{
}

bool ChunkProviderServer::isInsideStructure(World *worldIn, std::string_view structureName, BlockPos &pos) const
{
    return chunkGenerator->isInsideStructure(worldIn, structureName, pos);
}

int32_t ChunkProviderServer::getLoadedChunkCount() const
{
    return loadedChunks.size();
}

bool ChunkProviderServer::chunkExists(int32_t x, int32_t z)
{
    return loadedChunks.find(ChunkPos::asLong(x, z)) != loadedChunks.end();
}

bool ChunkProviderServer::isChunkGeneratedAt(int32_t x, int32_t z)
{
    return loadedChunks.find(ChunkPos::asLong(x, z)) != loadedChunks.end() || chunkLoader->isChunkGeneratedAt(x, z);
}

std::string ChunkProviderServer::makeString()
{
    return "ServerChunkCache: " + std::to_string(loadedChunks.size()) +
           " Drop: " + std::to_string(droppedChunks.size());
}

Chunk *ChunkProviderServer::loadChunkFromFile(int32_t x, int32_t z)
{
    try
    {
        auto chunk = chunkLoader->loadChunk(world, x, z);
        if (chunk != nullptr)
        {
            chunk.setLastSaveTime(world->getTotalWorldTime());
            chunkGenerator->recreateStructures(chunk, x, z);
        }

        return chunk;
    }
    catch (Exception var4)
    {
        LOGGER->error("Couldn't load chunk", var4);
        return nullptr;
    }
}

void ChunkProviderServer::saveChunkExtraData(Chunk *chunkIn)
{
    try
    {
        chunkLoader->saveExtraChunkData(world, chunkIn);
    }
    catch (Exception var3)
    {
        LOGGER->error("Couldn't save entities", var3);
    }
}

void ChunkProviderServer::saveChunkData(Chunk *chunkIn)
{
    try
    {
        chunkIn->setLastSaveTime(world->getTotalWorldTime());
        chunkLoader->saveChunk(world, chunkIn);
    }
    catch (IOException var3)
    {
        LOGGER->error("Couldn't save chunk", var3);
    }
    catch (MinecraftException var4)
    {
        LOGGER->error("Couldn't save chunk; already in use by another instance of Minecraft?", var4);
    }
}
