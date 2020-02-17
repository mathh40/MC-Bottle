#include "NBTBase.h"
#include "NBTTagByte.h"
#include "NBTTagEnd.h"
#include "NBTTagDouble.h"
#include "NBTTagByteArray.h"
#include "NBTTagCompound.h"
#include "NBTTagShort.h"
#include "NBTTagInt.h"
#include "NBTTagLong.h"
#include "NBTTagFloat.h"
#include "NBTTagString.h"
#include "NBTTagIntArray.h"
#include "NBTTagLongArray.h"

std::string NBTBase::NBT_TYPES[] = { "END", "BYTE", "SHORT", "INT", "LONG", "FLOAT", "DOUBLE", "BYTE[]", "STRING", "LIST", "COMPOUND", "INT[]", "LONG[]" };


std::shared_ptr<NBTBase> NBTBase::create(uint8_t id)
{
	switch (id) {
	case 0:
		return std::make_shared<NBTTagEnd>();
	case 1:
		return std::make_shared < NBTTagByte>();
	case 2:
		return std::make_shared < NBTTagShort>();
	case 3:
		return std::make_shared < NBTTagInt>();
	case 4:
		return std::make_shared < NBTTagLong>();
	case 5:
		return std::make_shared < NBTTagFloat>();
	case 6:
		return std::make_shared < NBTTagDouble>();
	case 7:
		return std::make_shared < NBTTagByteArray>();
	case 8:
		return std::make_shared < NBTTagString>();
	case 9:
		return std::make_shared < NBTTagList>();
	case 10:
		return std::make_shared < NBTTagCompound>();
	case 11:
		return std::make_shared < NBTTagIntArray>();
	case 12:
		return std::make_shared < NBTTagLongArray>();
	default:
		return nullptr;
	}
}

std::string_view NBTBase::getTypeName(int id)
{
	switch (id) {
	case 0:
		return "TAG_End";
	case 1:
		return "TAG_Byte";
	case 2:
		return "TAG_Short";
	case 3:
		return "TAG_Int";
	case 4:
		return "TAG_Long";
	case 5:
		return "TAG_Float";
	case 6:
		return "TAG_Double";
	case 7:
		return "TAG_Byte_Array";
	case 8:
		return "TAG_String";
	case 9:
		return "TAG_List";
	case 10:
		return "TAG_Compound";
	case 11:
		return "TAG_Int_Array";
	case 12:
		return "TAG_Long_Array";
	case 99:
		return "Any Numeric Tag";
	default:
		return "UNKNOWN";
	}
}

bool NBTBase::isEmpty() const
{
	return false;
}

std::string NBTBase::getString() const
{
	return to_string();
}

bool operator==(const NBTBase& a, const NBTBase& b)
{
	return a.getId() == b.getId();
}
