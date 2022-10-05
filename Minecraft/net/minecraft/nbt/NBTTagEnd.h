#pragma once
#include "NBTBase.h"

class NBTTagEnd : public NBTBase
{
  public:
    void read(std::istream &input, int depth, NBTSizeTracker sizeTracker) override;
    void write(std::ostream &output) override;
    uint8_t getId() const override;
    std::string to_string() const override;
};
