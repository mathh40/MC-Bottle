#include "NBTTagEnd.h"

#include "NBTSizeTracker.h"

void NBTTagEnd::read(std::istream &input, int depth, NBTSizeTracker sizeTracker)
{
    sizeTracker.read(64L);
}

void NBTTagEnd::write(std::ostream &output)
{
}

uint8_t NBTTagEnd::getId() const
{
    return 0;
}

std::string NBTTagEnd::to_string() const
{
    return "END";
}
