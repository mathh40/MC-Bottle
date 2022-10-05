#include "../../util/ResourceLocation.h"
#include "../ICriterionTrigger.h"
#include "../PlayerAdvancements.h"
#include "AbstractCriterionInstance.h"

class ConstructBeaconTrigger : public ICriterionTrigger
{
  public:
    class Listeners
    {
      public:
        Listeners(PlayerAdvancements playerAdvancementsIn);
        bool isEmpty() const;
        void add(ICriterionTrigger::Listener *listener);
        void remove(ICriterionTrigger::Listener *listener);
        void trigger(TileEntityBeacon *beacon);

      private:
        PlayerAdvancements playerAdvancements;
        std::set<ICriterionTrigger::Listener *> listeners;
    };

    class Instance : public AbstractCriterionInstance
    {
      public:
        Instance(MinMaxBounds level);
        bool test(TileEntityBeacon *beacon);

      private:
        MinMaxBounds level;
    };

    ResourceLocation &getId() const override;
    void addListener(PlayerAdvancements playerAdvancementsIn, ICriterionTrigger::Listener *listener) override;
    void removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2) override;
    void removeAllListeners(PlayerAdvancements var1) override;
    std::unique_ptr<ICriterionInstance> deserializeInstance(nlohmann::json &var1) override;
    void trigger(EntityPlayerMP *player, TileEntityBeacon *beacon);

  private:
    static ResourceLocation ID = ResourceLocation("construct_beacon");
    std::unordered_map<PlayerAdvancements, ConstructBeaconTrigger::Listeners *> listeners;
};
