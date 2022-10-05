#pragma once

class ByteBuffer;
class ICriterionInstance;

class Criterion
{
  public:
    Criterion(ICriterionInstance *p_i47470_1_);
    Criterion();
    void serializeToNetwork(ByteBuffer &p_192140_1_);
    static Criterion criterionFromJson(Json &json);
    static Criterion criterionFromNetwork(ByteBuffer &p_192146_0_);
    static std::unordered_map<> &criteriaFromJson(Json &json);
    static std::unordered_map<> &criteriaFromNetwork(ByteBuffer &bus);
    static void serializeToNetwork(const std::unordered_map<> &criteria, ByteBuffer &buf);
    ICriterionInstance *getCriterionInstance();

  private:
    ICriterionInstance *criterionInstance;
};
