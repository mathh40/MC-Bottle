#include "Mirror.h"
#include "Rotation.h"
#include "TileEntity.h"


class TileEntityStructure : public TileEntity
{
public:
    class Mode
    {
    public:
        static Mode SAVE;
        static Mode LOAD;
        static Mode CORNER;
        static Mode DATA;
        std::string getName() const;
        int32_t getModeId() const;
        static TileEntityStructure::Mode *getById(int32_t id);

        Mode(std::string modeNameIn, int32_t modeIdIn);

    private:
        static std::vector<Mode *> MODES;
        std::string modeName;
        int32_t modeId;
    };

    TileEntityStructure();
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    void readFromNBT(NBTTagCompound *compound) override;
    std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
    NBTTagCompound *getUpdateTag() override;
    bool usedBy(EntityPlayer *player);
    std::string getName() const;
    void setName(std::string_view nameIn);
    void createdBy(EntityLivingBase *p_189720_1_);
    BlockPos getPosition() const;
    void setPosition(BlockPos posIn);
    BlockPos getStructureSize() const;
    void setSize(BlockPos sizeIn);
    Mirror getMirror() const;
    void setMirror(Mirror mirrorIn);
    Rotation getRotation() const;
    void setRotation(Rotation rotationIn);
    std::string getMetadata() const;
    void setMetadata(std::string metadataIn);
    TileEntityStructure::Mode getMode();
    void setMode(TileEntityStructure::Mode modeIn);
    void nextMode() const;
    bool ignoresEntities() const;
    void setIgnoresEntities(bool ignoreEntitiesIn);
    float getIntegrity() const;
    void setIntegrity(float integrityIn);
    int64_t getSeed() const;
    void setSeed(int64_t seedIn);
    bool detectSize();
    void writeCoordinates(ByteBuffer &buf);
    bool save();
    bool load();
    void unloadStructure();
    bool isStructureLoadable();
    bool isPowered() const;
    void setPowered(bool poweredIn);
    bool showsAir() const;
    void setShowAir(bool showAirIn);
    bool showsBoundingBox() const;
    void setShowBoundingBox(bool showBoundingBoxIn);
    ITextComponent *getDisplayName() override;

private:
    void updateBlockState();
    std::vector<TileEntityStructure *> filterRelatedCornerBlocks(std::vector<TileEntityStructure *> p_184415_1_);
    std::vector<TileEntityStructure *> getNearbyCornerBlocks(BlockPos p_184418_1_, BlockPos p_184418_2_);
    StructureBoundingBox calculateEnclosingBoundingBox(BlockPos p_184416_1_,
                                                       std::vector<TileEntityStructure *> p_184416_2_);
    bool save(boolean writeToDisk);
    bool load(bool requireMatchingSize);

    std::string name = "";
    std::string author = "";
    std::string metadata = "";
    BlockPos position = BlockPos(0, 1, 0);
    BlockPos size;
    Mirror mirror;
    Rotation rotation;
    TileEntityStructure::Mode mode;
    bool ignoreEntities;
    bool powered;
    bool showAir;
    bool showBoundingBox;
    float integrity;
    int64_t seed;
};
