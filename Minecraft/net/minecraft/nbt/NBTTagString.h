#pragma once
#include "NBTBase.h"

class NBTTagString : public NBTBase
{
  public:
    NBTTagString() = default;
    NBTTagString(std::string data);
    void write(std::ostream &output) const override;
    void read(std::istream &input, int var2, NBTSizeTracker *sizeTracker) override;
    uint8_t getId() const override;
    std::string to_string() const override;

    bool isEmpty() const override;

    friend bool operator==(const NBTTagString &a, const NBTTagString &b);

    std::string getString() const override;

    std::string quoteAndEscape(std::string p_193588_0_) const;

  private:
    std::string data;
};
