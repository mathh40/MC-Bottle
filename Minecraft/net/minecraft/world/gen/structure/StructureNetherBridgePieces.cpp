#include "StructureNetherBridgePieces.h"

#include "MapGenStructureIO.h"

void StructureNetherBridgePieces::registerNetherFortressPieces()
{
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Crossing3.class, "NeBCr");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::End.class, "NeBEF");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Straight.class, "NeBS");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Corridor3.class, "NeCCS");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Corridor4.class, "NeCTB");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Entrance.class, "NeCE");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Crossing2.class, "NeSCSC");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Corridor.class, "NeSCLT");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Corridor5.class, "NeSC");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Corridor2.class, "NeSCRT");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::NetherStalkRoom.class, "NeCSR");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Throne.class, "NeMT");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Crossing.class, "NeRC");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Stairs.class, "NeSR");
    MapGenStructureIO::registerStructureComponent(StructureNetherBridgePieces::Start.class, "NeStart");
}

StructureNetherBridgePieces::Piece StructureNetherBridgePieces::findAndCreateBridgePieceFactory(
    StructureNetherBridgePieces::PieceWeight p_175887_0_, std::vector<> p_175887_1_, pcg32 &p_175887_2_,
    int32_t p_175887_3_, int32_t p_175887_4_, int32_t p_175887_5_, EnumFacing p_175887_6_, int32_t p_175887_7_)
{
    Class oclass                                                        = p_175887_0_.weightClass;
    StructureNetherBridgePieces.Piece structurenetherbridgepieces$piece = nullptr;
    if (oclass == StructureNetherBridgePieces.Straight.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Straight.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Crossing3.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Crossing3.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Crossing.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Crossing.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Stairs.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Stairs.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_7_, p_175887_6_);
    }
    else if (oclass == StructureNetherBridgePieces.Throne.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Throne.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_7_, p_175887_6_);
    }
    else if (oclass == StructureNetherBridgePieces.Entrance.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Entrance.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Corridor5.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Corridor5.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Corridor2.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Corridor2.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Corridor.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Corridor.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Corridor3.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Corridor3.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Corridor4.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Corridor4.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.Crossing2.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.Crossing2.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }
    else if (oclass == StructureNetherBridgePieces.NetherStalkRoom.class)
    {
        structurenetherbridgepieces$piece = StructureNetherBridgePieces.NetherStalkRoom.createPiece(
            p_175887_1_, p_175887_2_, p_175887_3_, p_175887_4_, p_175887_5_, p_175887_6_, p_175887_7_);
    }

    return (StructureNetherBridgePieces.Piece)structurenetherbridgepieces$piece;
}
