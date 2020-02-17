#include "StructureComponent.h"
#include "MapGenMineshaft.h"
class Peice;
class Stairs;
class Room;

class StructureMineshaftPieces
{
public:
	static void registerStructurePieces();
	
	class Peice : public StructureComponent
	{
	public:
		Peice() = default;
		Peice(int32_t p_i47138_1_, MapGenMineshaft::Type p_i47138_2_);
	protected:
		MapGenMineshaft::Type mineShaftType;
		void writeStructureToNBT(NBTTagCompound* tagCompound) override;
		void readStructureFromNBT(NBTTagCompound* tagCompound, const TemplateManager& p_143011_2_) override;
		bool isSupportingBox(World* p_189918_1_, StructureBoundingBox p_189918_2_, int32_t p_189918_3_, int32_t p_189918_4_, int32_t p_189918_5_, int32_t p_189918_6_);
		IBlockState* getPlanksBlock();
		IBlockState* getFenceBlock();
	private:
	};

	class Stairs : public StructureMineshaftPieces::Peice
	{
	public:
		Stairs() = default;
		Stairs(int32_t p_i47136_1_, pcg32& p_i47136_2_, StructureBoundingBox p_i47136_3_, EnumFacing p_i47136_4_, MapGenMineshaft::Type p_i47136_5_);
		void buildComponent(StructureComponent* componentIn, std::vector<> listIn, pcg32& rand);
		bool addComponentParts(World* worldIn, pcg32& randomIn, StructureBoundingBox structureBoundingBoxIn) override;
		
		static std::optional<StructureBoundingBox> findStairs(std::vector<> listIn, pcg32& rand, int32_t x, int32_t y, int32_t z, EnumFacing facing);
	private:
	};

	class Room : public StructureMineshaftPieces::Peice
	{
	public:
		Room() = default;
		Room(int32_t p_i47137_1_, pcg32& p_i47137_2_, int32_t p_i47137_3_, int32_t p_i47137_4_, MapGenMineshaft::Type p_i47137_5_);

		void buildComponent(StructureComponent* componentIn, std::vector<> listIn, pcg32& rand);
		bool addComponentParts(World* worldIn, pcg32& randomIn, StructureBoundingBox structureBoundingBoxIn) override;
		void offset(int32_t x, int32_t y, int32_t z) override;
	protected:
		void writeStructureToNBT(NBTTagCompound* tagCompound) override;
		void readStructureFromNBT(NBTTagCompound* tagCompound, const TemplateManager& p_143011_2_) override;
	private:
		std::vector<StructureBoundingBox> connectedRooms;
	};

	class Cross : public StructureMineshaftPieces::Peice
	{
	public:
		Cross() = default;
		Cross(int32_t p_i47139_1_, pcg32& p_i47139_2_, StructureBoundingBox p_i47139_3_, std::optional<EnumFacing> p_i47139_4_, MapGenMineshaft::Type p_i47139_5_);
		std::optional<StructureBoundingBox> findCrossing(std::vector<> listIn, pcg32& rand, int32_t x, int32_t y, int32_t z, EnumFacing facing);
		void buildComponent(StructureComponent* componentIn, std::vector<> listIn, pcg32& rand);
		bool addComponentParts(World* worldIn, pcg32& randomIn, StructureBoundingBox structureBoundingBoxIn) override;
	protected:
		void writeStructureToNBT(NBTTagCompound* tagCompound) override;
		void readStructureFromNBT(NBTTagCompound* tagCompound, const TemplateManager& p_143011_2_) override;
	private:
		EnumFacing corridorDirection;
		bool isMultipleFloors;

		void placeSupportPillar(World* p_189923_1_, StructureBoundingBox p_189923_2_, int32_t p_189923_3_, int32_t p_189923_4_, int32_t p_189923_5_, int32_t p_189923_6_);
	};

	class Corridor : public StructureMineshaftPieces::Peice
	{
	public:
		Corridor() = default;
		Corridor(int32_t p_i47140_1_, pcg32& p_i47140_2_, StructureBoundingBox p_i47140_3_, EnumFacing p_i47140_4_, MapGenMineshaft::Type p_i47140_5_);
		std::optional<StructureBoundingBox> findCorridorSize(std::vector<> p_175814_0_, pcg32& rand, int32_t x, int32_t y, int32_t z, EnumFacing facing);
		void buildComponent(StructureComponent* componentIn, std::vector<> listIn, pcg32& rand);
		bool addComponentParts(World* worldIn, pcg32& randomIn, StructureBoundingBox structureBoundingBoxIn) override;
	protected:
		void writeStructureToNBT(NBTTagCompound* tagCompound) override;
		void readStructureFromNBT(NBTTagCompound* tagCompound, const TemplateManager& p_143011_2_) override;;
		bool generateChest(World* worldIn, StructureBoundingBox structurebb, pcg32& randomIn, int32_t x, int32_t y, int32_t z, ResourceLocation loot) override;
	private:
		bool hasRails;
		bool hasSpiders;
		bool spawnerPlaced;
		int32_t sectionCount;

		void placeSupport(World* p_189921_1_, StructureBoundingBox p_189921_2_, int32_t p_189921_3_, int32_t p_189921_4_, int32_t p_189921_5_, int32_t p_189921_6_, int32_t p_189921_7_, pcg32& p_189921_8_);
		void placeCobWeb(World* p_189922_1_, StructureBoundingBox p_189922_2_, pcg32& p_189922_3_, float p_189922_4_, int32_t p_189922_5_, int32_t p_189922_6_, int32_t p_189922_7_);
	};
	
private:
	static std::unique_ptr<StructureMineshaftPieces::Peice> createRandomShaftPiece(std::vector<> p_189940_0_, pcg32& p_189940_1_, int32_t p_189940_2_, int32_t p_189940_3_, int32_t p_189940_4_, std::optional<EnumFacing> p_189940_5_, int32_t p_189940_6_, MapGenMineshaft::Type p_189940_7_);
	static std::unique_ptr<StructureMineshaftPieces::Peice> generateAndAddPiece(StructureComponent* p_189938_0_, std::vector<> p_189938_1_, pcg32& p_189938_2_, int32_t p_189938_3_, int32_t p_189938_4_, int32_t p_189938_5_, EnumFacing p_189938_6_, int32_t p_189938_7_);
};