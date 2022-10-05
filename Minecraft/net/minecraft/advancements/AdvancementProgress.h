#pragma once

#include <string>
#include <unordered_map>

class ByteBuffer;

class AdvancementProgress
{
  public:
    void update(std::unordered_map<> criteriaIn, std::string[][] requirements);
    bool isDone();
    bool hasProgress();
    bool grantCriterion(std::string_view criterionIn);
    bool revokeCriterion(std::string_view criterionIn);
    std::string toString();
    void serializeToNetwork(ByteBuffer &p_192104_1_);
    static AdvancementProgress fromNetwork(ByteBuffer &p_192100_0_);
    CriterionProgress getCriterionProgress(std::string_view criterionIn);
    float getPercent();
    std::string getProgressText();
    Iterable getRemaningCriteria();
    Iterable getCompletedCriteria();
    Date getFirstProgressDate();

  private:
    uint32_t countCompletedRequirements();
    std::unordered_map<> criteria;
    std::string[][] requirements;
};

using json = nlohmann::json;

namespace ns
{
void to_json(json &j, const AdvancementProgress &p)
{
}

void from_json(const json &j, AdvancementProgress &p)
{
}
} // namespace ns
