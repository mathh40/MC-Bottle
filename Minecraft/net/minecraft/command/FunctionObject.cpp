#include "FunctionObject.h"
#include "fmt/format.h"
#include "StringUtils.h"
#include "Util.h"

std::string FunctionObject::CommandEntry::toString() const {
    return "/" + command;
}

FunctionObject::CacheableFunction::CacheableFunction(std::optional<ResourceLocation> idIn):
    id(idIn) {
}

FunctionObject::CacheableFunction::CacheableFunction(FunctionObject functionIn):
    id(std::nullopt), function(functionIn) {
}

std::optional<FunctionObject> FunctionObject::CacheableFunction::get(const FunctionManager &functionManagerIn) {
    if (!isValid) {
        if (id.has_value()) {
            function = functionManagerIn.getFunction(id);
        }

        isValid = true;
    }

    return function;
}

std::string FunctionObject::CacheableFunction::toString() const {
    return fmt::format("{}",id);
}

FunctionObject::FunctionObject(const std::vector<std::unique_ptr<FunctionObject::Entry>> &entriesIn):
    entries(entriesIn) {
}

FunctionObject::FunctionEntry::FunctionEntry(const FunctionObject &functionIn):
    function(functionIn) {
}

void FunctionObject::FunctionEntry::execute(const FunctionManager &functionManagerIn, ICommandSender *sender,
                                            const std::vector<> commandQueue, int32_t maxCommandChainLength) {
    FunctionObject functionobject = function.get(functionManagerIn);
    if (functionobject != nullptr) {
        auto entry = functionobject.getEntries();
        int32_t i = maxCommandChainLength - commandQueue.size();
        int32_t j = MathHelper::min(entry.size(), i);

        for (auto k = j - 1; k >= 0; --k) {
            commandQueue.addFirst(new FunctionManager::QueuedCommand(functionManagerIn, sender, entry[k]));
        }
    }

}

std::string FunctionObject::FunctionEntry::toString() const{
    return "/function " + function;
}

FunctionObject::CommandEntry::CommandEntry(std::string p_i47534_1_):
    command(p_i47534_1_) {
}

void FunctionObject::CommandEntry::execute(const FunctionManager &functionManagerIn, ICommandSender *sender,
                                           const std::vector<> commandQueue, int32_t maxCommandChainLength) {
    functionManagerIn.getCommandManager().executeCommand(sender, command);
}

std::vector<std::unique_ptr<FunctionObject::Entry>> &FunctionObject::getEntries() {
    return entries;
}

FunctionObject FunctionObject::create(const FunctionManager &functionManagerIn, const std::vector<std::string_view> &commands) {
    std::vector<std::unique_ptr<FunctionObject::Entry>> list;
    list.resize(commands.size());

    for(auto s : commands){
        std::string temp(s);
        StringUtils::trim(temp);
        if (!Util::startwith("#",s) && !s.empty()) {
            auto astring = Util::split(s, " ", 2);
            std::string s1 = astring[0];
            if (!functionManagerIn.getCommandManager().getCommands().containsKey(s1)) {
                if (Util::startwith("//" , s1)) {
                    throw std::logic_error(
                        "Unknown or invalid command '" + s1 +
                        "' (if you intended to make a comment, use '#' not '//')");
                }

                if (Util::startwith("/",s1) && s1.size() > 1) {
                    throw std::logic_error(
                        "Unknown or invalid command '" + s1 + "' (did you mean '" + s1.substr(1) +
                        "'? Do not use a preceding forwards slash.)");
                }

                throw std::logic_error("Unknown or invalid command '" + s1 + "'");
            }

            list.emplace_pack(std::make_unique<FunctionObject::CommandEntry>(s));
        }
    }

    return FunctionObject(list);
}

FunctionObject::CacheableFunction FunctionObject::CacheableFunction::EMPTY = FunctionObject::CacheableFunction(std::nullopt);
