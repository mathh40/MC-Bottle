#include "AbstractCriterionInstance.h"
#include "../../util/ResourceLocation.h"
#include "../ICriterionTrigger.h"
#include "../PlayerAdvancements.h"

class EntityPlayerMP;

class ChangeDimensionTrigger :public ICriterionTrigger
{
    public:

        class Listeners
      {
        public:
            Listeners(PlayerAdvancements playerAdvancementsIn);
            bool isEmpty() const;
            void add(ICriterionTrigger::Listener* listener);
            void remove(ICriterionTrigger::Listener* listener);
            void trigger(DimensionType from, DimensionType to);

      private:
        PlayerAdvancements playerAdvancements;
        std::set<ChangeDimensionTrigger::Listeners*> listeners;
      };

        class Instance :public AbstractCriterionInstance
      {
        public:
            Instance(DimensionType from, DimensionType to);
            bool test(DimensionType from, DimensionType to);

      private:
          DimensionType from;
          DimensionType to;
        };

        ResourceLocation& getId() const override;
        void addListener(PlayerAdvancements var1, ICriterionTrigger::Listener* var2) override;
        void removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener* var2) override;
        void removeAllListeners(PlayerAdvancements var1) override;
        std::unique_ptr<ICriterionInstance> deserializeInstance(nlohmann::json& var1) override;
        void trigger(EntityPlayerMP* player, DimensionType from, DimensionType to);

private:
        static ResourceLocation ID = ResourceLocation("changed_dimension");
        std::unordered_map<PlayerAdvancements,ChangeDimensionTrigger::Listeners*> listeners; listeners;
};
