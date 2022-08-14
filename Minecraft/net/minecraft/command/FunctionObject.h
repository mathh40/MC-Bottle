#pragma once
#include "math/MathHelper.h"

#include <memory>
#include <optional>
#include <vector>
class ResourceLocation;
class ICommandSender;
class FunctionManager;
class FunctionObject {

public:
       class Entry {
       public:
           virtual ~Entry() = default;
           virtual void execute(const FunctionManager& functionManagerIn, ICommandSender* sender, const std::vector<> commandQueue, int32_t maxCommandChainLength) = 0;
   };

       class FunctionEntry :public FunctionObject::Entry {
       public:
           FunctionEntry(const FunctionObject &functionIn);

           void execute(const FunctionManager &functionManagerIn, ICommandSender *sender,
                        const std::vector<> commandQueue, int32_t maxCommandChainLength) override;

           std::string toString() const;

           private:
           FunctionObject::CacheableFunction function;
   };

    class CommandEntry :public FunctionObject::Entry {
    public:
        CommandEntry(std::string p_i47534_1_);

        void execute(const FunctionManager &functionManagerIn, ICommandSender *sender,
                     const std::vector<> commandQueue, int32_t maxCommandChainLength) override;

        std::string toString() const;

    private:
        std::string command;
   };

    class CacheableFunction {
    public:
        static FunctionObject::CacheableFunction EMPTY;
        explicit CacheableFunction(std::optional<ResourceLocation> idIn);
        explicit CacheableFunction(FunctionObject functionIn);
        std::optional<FunctionObject> get(const FunctionManager &functionManagerIn);
        std::string toString() const;
    private:
        std::optional<ResourceLocation> id;
        bool isValid;
        std::optional<FunctionObject> function;
   };

    FunctionObject(const std::vector<std::unique_ptr<FunctionObject::Entry>> &entriesIn);
    std::vector<std::unique_ptr<FunctionObject::Entry>> &getEntries();
    static FunctionObject create(const FunctionManager &functionManagerIn, const std::vector<std::string_view> &commands);
private :
    std::vector<std::unique_ptr<FunctionObject::Entry>> entries;
};
