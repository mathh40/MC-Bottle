#pragma once

class ByteBuffer;
class CriterionProgress
{
  public:
    CriterionProgress(const AdvancementProgress &advancementProgressIn);
    bool isObtained();
    void obtain();
    void reset();
    Date getObtained();
    std::string toString();
    void write(ByteBuffer &buf);
    json serialize();
    static CriterionProgress read(ByteBuffer &buf, const AdvancementProgress &advancementProgressIn);
    static CriterionProgress fromDateTime(const AdvancementProgress &advancementProgressIn, std::string_view dateTime);

  private:
    static SimpleDateFormat DATE_TIME_FORMATTER;
    AdvancementProgress *advancementProgress;
    Date obtained;
};
