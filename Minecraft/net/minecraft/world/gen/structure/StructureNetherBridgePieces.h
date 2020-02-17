#include <vector>
#include "../../../../../../pcg-cpp/pcg_random.hpp"

class EnumFacing;

class StructureNetherBridgePieces
{
public:
	void registerNetherFortressPieces();

	class Throne :public StructureNetherBridgePieces::Piece
	{
	public:
		Throne()=default;
	private:
		bool hasSpawner;
	};
private:
	static const StructureNetherBridgePieces::PieceWeight PRIMARY_COMPONENTS[] = new StructureNetherBridgePieces.PieceWeight[]{new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Straight.class, 30, 0, true), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Crossing3.class, 10, 4), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Crossing.class, 10, 4), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Stairs.class, 10, 3), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Throne.class, 5, 2), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Entrance.class, 5, 1)};
	static const StructureNetherBridgePieces::PieceWeight SECONDARY_COMPONENTS[] = new StructureNetherBridgePieces.PieceWeight[]{new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Corridor5.class, 25, 0, true), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Crossing2.class, 15, 5), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Corridor2.class, 5, 10), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Corridor.class, 5, 10), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Corridor3.class, 10, 3, true), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.Corridor4.class, 7, 2), new StructureNetherBridgePieces.PieceWeight(StructureNetherBridgePieces.NetherStalkRoom.class, 5, 2)};

	StructureNetherBridgePieces::Piece findAndCreateBridgePieceFactory(StructureNetherBridgePieces::PieceWeight p_175887_0_, std::vector<> p_175887_1_, pcg32& p_175887_2_, int32_t p_175887_3_, int32_t p_175887_4_, int32_t p_175887_5_, EnumFacing p_175887_6_, int32_t p_175887_7_);
};
