#pragma once
#include "NBTBase.h"

class NBTPrimitive : public NBTBase
{
  public:
    virtual int64_t getLong() const;

    virtual int32_t getInt() const;

    virtual int16_t getShort() const;

    virtual uint8_t getByte() const;

    virtual double getDouble() const;

    virtual float getFloat() const;
};
