#include "CompressedStreamTools.h"

#include "../util/ReportedException.h"
#include "NBTBase.h"
#include "NBTSizeTracker.h"
#include "NBTTagEnd.h"
#include "Util.h"
#include "gzstream.h"

namespace CompressedStreamTools
{
void writeTag(NBTBase *tag, std::ostream &output)
{
    auto id = tag->getId();
    output.write(reinterpret_cast<char *>(&id), sizeof(id));
    if (tag->getId() != 0)
    {
        output << "";
        tag->write(output);
    }
}

std::unique_ptr<NBTBase> read(std::istream &input, int depth, NBTSizeTracker *accounter)
{
    uint8_t b0;
    input.read(reinterpret_cast<char *>(&b0), sizeof(uint8_t));
    if (b0 == 0)
    {
        return std::make_unique<NBTTagEnd>();
    }
    else
    {
        char c;
        input >> c;
        auto nbtbase = NBTBase::create(b0);

        try
        {
            nbtbase->read(input, depth, accounter);
            return nbtbase;
        }
        catch (IOException var8)
        {
            CrashReport crashreport                 = CrashReport.makeCrashReport(var8, "Loading NBT data");
            CrashReportCategory crashreportcategory = crashreport.makeCategory("NBT Tag");
            crashreportcategory.addCrashSection("Tag type", b0);
            throw ReportedException(crashreport);
        }
    }
}

std::unique_ptr<NBTTagCompound> readCompressed(std::istream &is)
{
    std::shared_ptr<gzstreambuf> buf = std::make_shared<gzstreambuf>();
    is.rdbuf(buf.get());

    auto nbttagcompound = CompressedStreamTools::read(is, NBTSizeTracker::Infinite.get());

    return nbttagcompound;
}

void writeCompressed(NBTTagCompound *compound, std::ostream &outputStream)
{
    std::shared_ptr<gzstreambuf> buf = std::make_shared<gzstreambuf>();
    outputStream.rdbuf(buf.get());
    CompressedStreamTools::write(compound, outputStream);
}

void safeWrite(NBTTagCompound *compound, std::filesystem::path fileIn)
{
    auto file1 = std::filesystem::absolute(fileIn) += std::filesystem::path("_tmp");
    if (exists(file1))
    {
        std::filesystem::remove(file1);
    }

    CompressedStreamTools::write(compound, file1);
    if (exists(fileIn))
    {
        std::filesystem::remove(fileIn);
    }

    if (exists(fileIn))
    {
        throw std::runtime_error("Failed to delete " + fileIn.string());
    }
    else
    {
        rename(file1, fileIn);
    }
}

void write(NBTTagCompound *compound, std::filesystem::path fileIn)
{
    auto dataoutputstream = ogzstream(fileIn.string().c_str());

    write(compound, dataoutputstream);
}

std::unique_ptr<NBTTagCompound> read(std::filesystem::path fileIn)
{
    if (!exists(fileIn))
    {
        return std::make_unique<NBTTagCompound>();
    }
    else
    {
        auto datainputstream = igzstream(fileIn.string().c_str());

        auto nbttagcompound = read(datainputstream, NBTSizeTracker::Infinite.get());

        return nbttagcompound;
    }
}

std::unique_ptr<NBTTagCompound> read(std::istream &inputStream)
{
    read(inputStream, NBTSizeTracker::Infinite.get());
}

std::unique_ptr<NBTTagCompound> read(std::istream &input, NBTSizeTracker *accounter)
{
    auto nbtbase = read(input, 0, accounter);
    if (Util:: instanceof <NBTTagCompound>(nbtbase.get()))
    {
        return nbtbase;
    }
    else
    {
        throw std::logic_error("Root tag must be a named compound tag");
    }
}

void write(NBTTagCompound *compound, std::ostream &output)
{
    CompressedStreamTools::writeTag(compound, output);
}
} // namespace CompressedStreamTools
