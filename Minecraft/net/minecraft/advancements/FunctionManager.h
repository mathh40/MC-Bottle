#pragma once

class ICommandSender;
class ICommandManager;

class FunctionManager
{
  public:
    class QueuedCommand
    {
      public:
        QueuedCommand(FunctionManager *functionManagerIn, ICommandSender *senderIn, FunctionObject::Entry entryIn);
        void execute(ArrayDeque commandQueue, uint64_t maxCommandChainLength);
        std::string toString() const;

      private:
        FunctionManager *functionManager;
        ICommandSender *sender;
        FunctionObject::Entry entry;
    };

    FunctionManager(std::filesystem::patch functionDirIn, MinecraftServer *serverIn);
    FunctionObject &getFunction(ResourceLocation id) const;
    ICommandManager *getCommandManager() const;
    uint64_t getMaxCommandChainLength() const;
    std::unordered_map<> &getFunctions() const;
    void update();
    uint64_t execute(FunctionObject function, ICommandSender *sender);
    void reload();

  private:
    void loadFunctions();

    static std::shared_ptr<spdlog::logger> LOGGER = LogManager.getLogger();
    std::filesystem::patch functionDir;
    MinecraftServer *server;
    std::unordered_map<> &functions;
    std::string currentGameLoopFunctionId = "-";
    FunctionObject gameLoopFunction;
    ArrayDeque commandQueue;
    bool isExecuting = false;
    ICommandSender *gameLoopFunctionSender;
};
