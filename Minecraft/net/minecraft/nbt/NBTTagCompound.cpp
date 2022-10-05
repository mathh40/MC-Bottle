#include "NBTTagCompound.h"

#include "../../../../compile-time-regular-expressions/single-header/ctre.hpp"
#include "../util/ReportedException.h"
#include "NBTSizeTracker.h"
#include "NBTTagByteArray.h"
#include "NBTTagDouble.h"
#include "NBTTagFloat.h"
#include "NBTTagInt.h"
#include "NBTTagIntArray.h"
#include "NBTTagLong.h"
#include "NBTTagShort.h"
#include "NBTTagString.h"
#include "crossguid/guid.hpp"
#include "spdlog/spdlog.h"

#include <sstream>

static constexpr auto SIMPLE_VALUE                     = ctll::fixed_string{R"([A-Za-z0-9._+-]+)"};
std::shared_ptr<spdlog::logger> NBTTagCompound::LOGGER = spdlog::get("Minecraft")->clone("NBTTagCompound");

void NBTTagCompound::write(std::ostream &output) const
{
    for (const auto &set : tagMap)
    {
        writeEntry(set.first, set.second.get(), output);
    }
    output << 0;
}

void NBTTagCompound::read(std::istream &input, int depth, NBTSizeTracker sizeTracker)
{
    sizeTracker.read(384L);
    if (depth > 512)
    {
        throw std::runtime_error("Tried to read NBT tag with too high complexity, depth > 512");
    }
    else
    {
        tagMap.clear();

        uint8_t b0;
        while ((b0 = readType(input, sizeTracker)) != 0)
        {
            auto s = readKey(input, sizeTracker);
            sizeTracker.read(static_cast<int64_t>(224 + 16 * s.length()));
            auto nbtbase = readNBT(b0, s, input, depth + 1, sizeTracker);
            tagMap.emplace(s, nbtbase);
            sizeTracker.read(288L);
        }
    }
}

uint8_t NBTTagCompound::getId() const
{
    return 10;
}

int32_t NBTTagCompound::getSize() const
{
    return tagMap.size();
}

std::string NBTTagCompound::to_string() const
{
    std::stringstream stringbuilder("{");
    std::vector<std::string> list;

    list.reserve(tagMap.size());

    for (auto &it : tagMap)
    {
        list.push_back(it.first);
    }

    if (LOGGER->level() == spdlog::level::debug)
    {
        std::sort(list.begin(), list.end());
    }

    for (const auto &s : list)
    {
        if (stringbuilder.str().size() != 1)
        {
            stringbuilder << (',');
        }

        stringbuilder << (handleEscape(s)) << (':') << (tagMap.at(s));
    }
    stringbuilder << ('}');

    return stringbuilder.str();
}

bool NBTTagCompound::isEmpty() const
{
    return tagMap.empty();
}

void NBTTagCompound::merge(NBTTagCompound *other)
{

    for (auto s : other->tagMap)
    {
        auto nbtbase = other->tagMap[s.first];
        if (nbtbase->getId() == 10)
        {
            if (hasKey(s, 10))
            {
                auto nbttagcompound = getCompoundTag(s);
                nbttagcompound->merge((NBTTagCompound)nbtbase);
            }
            else
            {
                setTag(s, nbtbase.copy());
            }
        }
        else
        {
            setTag(s, nbtbase.copy());
        }
    }
}

void NBTTagCompound::setTag(std::string key, std::unique_ptr<NBTBase> value)
{
    tagMap.emplace(key, value);
}

void NBTTagCompound::setByte(std::string key, uint8_t value)
{
    tagMap.emplace(key, std::make_shared<NBTTagByte>(value));
}

void NBTTagCompound::setShort(std::string key, int16_t value)
{
    tagMap.emplace(key, std::make_shared<NBTTagShort>(value));
}

void NBTTagCompound::setInteger(std::string key, int32_t value)
{
    tagMap.emplace(key, std::make_shared<NBTTagInt>(value));
}

void NBTTagCompound::setLong(std::string key, int64_t value)
{
    tagMap.emplace(key, std::make_shared<NBTTagLong>(value));
}

void NBTTagCompound::setUniqueId(std::string key, const xg::Guid &value)
{
    auto bytes         = value.bytes();
    const char most[]  = {bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]};
    const char least[] = {bytes[8], bytes[9], bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15]};
    setLong(key + "Most", reinterpret_cast<int64_t>(&most));
    setLong(key + "Least", reinterpret_cast<int64_t>(&least));
}

xg::Guid NBTTagCompound::getUniqueId(std::string key) const
{
    auto m             = getLong(key + "Most");
    auto l             = getLong(key + "Least");
    const auto most    = reinterpret_cast<char *>(m);
    const auto least   = reinterpret_cast<char *>(l);
    const char value[] = {most[0],  most[1],  most[2],  most[3],  most[4],  most[5],  most[6],  most[7],
                          least[0], least[1], least[2], least[3], least[4], least[5], least[6], least[7]};
    return xg::Guid(value);
}

bool NBTTagCompound::hasUniqueId(std::string key) const
{
    return hasKey(key + "Most", 99) && hasKey(key + "Least", 99);
}

void NBTTagCompound::setFloat(std::string key, float value)
{
    tagMap.emplace(key, std::make_shared<NBTTagFloat>(value));
}

void NBTTagCompound::setDouble(std::string key, double value)
{
    tagMap.emplace(key, std::make_shared<NBTTagDouble>(value));
}

void NBTTagCompound::setString(std::string key, std::string value)
{
    tagMap.emplace(key, std::make_shared<NBTTagString>(value));
}

void NBTTagCompound::setByteArray(std::string key, ByteBuffer &value)
{
    tagMap.emplace(key, std::make_shared<NBTTagByteArray>(value));
}

void NBTTagCompound::setIntArray(std::string key, std::vector<int32_t> &value)
{
    tagMap.emplace(key, std::make_shared<NBTTagIntArray>(value));
}

void NBTTagCompound::setBoolean(std::string key, bool value)
{
    setByte(key, static_cast<uint8_t>(value ? 1 : 0));
}

NBTBase *NBTTagCompound::getTag(std::string key) const
{
    return tagMap.at(key).get();
}

uint8_t NBTTagCompound::getTagId(std::string key) const
{
    const auto nbtbase = tagMap.at(key);
    return nbtbase == nullptr ? 0 : nbtbase->getId();
}

bool NBTTagCompound::hasKey(std::string key) const
{
    return tagMap.find(key) != tagMap.end();
}

bool NBTTagCompound::hasKey(std::string key, int type) const
{
    int i = getTagId(key);
    if (i == type)
    {
        return true;
    }
    else if (type != 99)
    {
        return false;
    }
    else
    {
        return i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6;
    }
}

uint8_t NBTTagCompound::getByte(std::string key) const
{
    if (hasKey(key, 99))
    {
        return dynamic_cast<NBTPrimitive *>(tagMap.at(key).get())->getByte();
    }

    return 0;
}

int16_t NBTTagCompound::getShort(std::string key) const
{
    if (hasKey(key, 99))
    {
        return dynamic_cast<NBTPrimitive *>(tagMap.at(key).get())->getShort();
    }

    return 0;
}

int32_t NBTTagCompound::getInteger(std::string key) const
{
    if (hasKey(key, 99))
    {
        return dynamic_cast<NBTPrimitive *>(tagMap.at(key).get())->getInt();
    }

    return 0;
}

int64_t NBTTagCompound::getLong(std::string key) const
{
    if (hasKey(key, 99))
    {
        return dynamic_cast<NBTPrimitive *>(tagMap.at(key).get())->getLong();
    }

    return 0;
}

float NBTTagCompound::getFloat(std::string key) const
{
    if (hasKey(key, 99))
    {
        return dynamic_cast<NBTPrimitive *>(tagMap.at(key).get())->getFloat();
    }

    return 0;
}

double NBTTagCompound::getDouble(std::string key) const
{
    if (hasKey(key, 99))
    {
        return dynamic_cast<NBTPrimitive *>(tagMap.at(key).get())->getDouble();
    }

    return 0;
}

std::string NBTTagCompound::getString(std::string key) const
{
    if (hasKey(key, 8))
    {
        return (tagMap.at(key))->getString();
    }

    return "";
}

ByteBuffer NBTTagCompound::getByteArray(std::string key) const
{
    if (hasKey(key, 7))
    {
        return dynamic_cast<NBTTagByteArray *>(tagMap.at(key).get())->getByteArray();
    }

    return ByteBuffer();
}

std::vector<int32_t> NBTTagCompound::getIntArray(std::string key) const
{
    if (hasKey(key, 11))
    {
        return dynamic_cast<NBTTagIntArray *>(tagMap.at(key).get())->getIntArray();
    }

    return std::vector<int32_t>();
}

NBTTagCompound *NBTTagCompound::getCompoundTag(std::string key) const
{
    if (hasKey(key, 10))
    {
        return dynamic_cast<NBTTagCompound *>(tagMap.at(key).get());
    }

    return nullptr;
}

NBTTagList *NBTTagCompound::getTagList(std::string key, int type) const
{
    if (getTagId(key) == 9)
    {
        auto nbttaglist = dynamic_cast<NBTTagList *>(tagMap.at(key).get());
        if (!nbttaglist->isEmpty() && nbttaglist->getTagType() != type)
        {
            return nullptr;
        }

        return nbttaglist;
    }

    return nullptr;
}

bool NBTTagCompound::getBoolean(std::string key) const
{
    return getByte(key) != 0;
}

void NBTTagCompound::removeTag(std::string key)
{
    tagMap.erase(key);
}

const std::unordered_map<const std::string, std::shared_ptr<NBTBase>> &NBTTagCompound::getCompoundMap() const
{
    return tagMap;
}

std::string NBTTagCompound::handleEscape(std::string p_193582_0_) const
{
    auto match = ctre::match<SIMPLE_VALUE>(p_193582_0_);
    return SIMPLE_VALUE.matcher(p_193582_0_).matches() ? p_193582_0_ : NBTTagString.quoteAndEscape(p_193582_0_);
}

void NBTTagCompound::writeEntry(std::string name, NBTBase *data, std::ostream &output) const
{
    auto id = data->getId();
    output.write(reinterpret_cast<char *>(&id), sizeof(id));
    if (id != 0)
    {
        output.write(name.data(), name.size());
        data->write(output);
    }
}

uint8_t NBTTagCompound::readType(std::istream &input, NBTSizeTracker sizeTracker) const
{
    uint8_t byte = 0;
    input.read(reinterpret_cast<char *>(&byte), sizeof(byte));
    return byte;
}

std::string NBTTagCompound::readKey(std::istream &input, NBTSizeTracker sizeTracker) const
{
    std::string a;
    input >> a;
    return a;
}

std::shared_ptr<NBTBase> NBTTagCompound::readNBT(uint8_t id, std::string key, std::istream &input, int depth,
                                                 NBTSizeTracker sizeTracker)
{
    auto nbtbase = NBTBase::create(id);

    try
    {
        nbtbase->read(input, depth, sizeTracker);
        return nbtbase;
    }
    catch (IOException &var9)
    {
        CrashReport crashreport                 = CrashReport.makeCrashReport(var9, "Loading NBT data");
        CrashReportCategory crashreportcategory = crashreport.makeCategory("NBT Tag");
        crashreportcategory.addCrashSection("Tag name", key);
        crashreportcategory.addCrashSection("Tag type", id);
        throw ReportedException(crashreport);
    }
}

CrashReport NBTTagCompound::createCrashReport(std::string key, int expectedType, ClassCastException ex)
{
    CrashReport crashreport                 = CrashReport.makeCrashReport(ex, "Reading NBT data");
    CrashReportCategory crashreportcategory = crashreport.makeCategoryDepth("Corrupt NBT tag", 1);
    crashreportcategory.addDetail("Tag type found", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return NBTBase.NBT_TYPES[((NBTBase)NBTTagCompound.this.tagMap.get(key)).getId()];
}
});
    crashreportcategory.addDetail("Tag type expected", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return NBTBase.NBT_TYPES[expectedType];
    }
    });
    crashreportcategory.addCrashSection("Tag name", key);
    return crashreport;
    }

    bool operator==(const NBTTagCompound &a, const NBTTagCompound &b)
    {
        return a.tagMap == b.tagMap;
    }
