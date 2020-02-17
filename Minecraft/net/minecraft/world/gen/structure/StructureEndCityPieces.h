#include <vector>
#include "math/BlockPos.h"
#include "../../../../../../pcg-cpp/pcg_random.hpp"
#include "StructureComponentTemplate.h"

class Rotation;
class TemplateManager;
class StructureEndCityPieces
{
public:

	class CityTemplate :public StructureComponentTemplate
	{
	public:
		CityTemplate() = default;
		CityTemplate(TemplateManager& p_i47214_1_, std::string_view p_i47214_2_, BlockPos& p_i47214_3_, Rotation p_i47214_4_, bool overwriteIn);
		void loadTemplate(const TemplateManager& p_191085_1_);
	protected:
		void writeStructureToNBT(NBTTagCompound* tagCompound) override;
		void readStructureFromNBT(NBTTagCompound* tagCompound, const TemplateManager& p_143011_2_) override;
		void handleDataMarker(std::string_view function, BlockPos& pos, World* worldIn, pcg32 rand, StructureBoundingBox sbb);
	private:
		std::string pieceName;
		Rotation rotation;
		bool overwrite;
	};
	
	class IGenerator
	{
	public:
		virtual ~IGenerator() = default;
		virtual void init() = 0;
		virtual bool generate(TemplateManager& var1, int32_t var2, CityTemplate& var3, BlockPos& var4, std::vector<StructureEndCityPieces::CityTemplate>& var5, pcg32& var6) = 0;
	};
	
	static void registerPieces();
	static void startHouseTower(TemplateManager& p_191087_0_, BlockPos& p_191087_1_, Rotation p_191087_2_, std::vector<CityTemplate>& p_191087_3_, pcg32& p_191087_4_);
private:
	static const PlacementSettings OVERWRITE = (PlacementSettings()).setIgnoreEntities(true);
	static const PlacementSettings INSERT;
	static const std::unique_ptr<IGenerator> HOUSE_TOWER_GENERATOR;
	static const std::vector<> TOWER_BRIDGES;
	static const std::unique_ptr<IGenerator> TOWER_GENERATOR;
	static const std::unique_ptr<IGenerator> TOWER_BRIDGE_GENERATOR;
	static const std::vector<> FAT_TOWER_BRIDGES;
	static const std::unique_ptr<IGenerator> FAT_TOWER_GENERATOR;

	static CityTemplate addPiece(TemplateManager& p_191090_0_, CityTemplate& p_191090_1_, BlockPos& p_191090_2_, std::string_view p_191090_3_, Rotation p_191090_4_, bool owerwrite);
	static CityTemplate addHelper(std::vector<CityTemplate>& p_189935_0_, CityTemplate& p_189935_1_);
	static bool recursiveChildren(TemplateManager& p_191088_0_, IGenerator& p_191088_1_, int32_t p_191088_2_, CityTemplate p_191088_3_, BlockPos& p_191088_4_, std::vector<> p_191088_5_, pcg32& p_191088_6_);
};