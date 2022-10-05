#pragma once
#include <optional>
#include <vector>

class TabCompleter
{
  public:
    TabCompleter(GuiTextField textFieldIn, bool hasTargetBlockIn);
    void complete();

    virtual std::optional<BlockPos> getTargetBlockPos() = 0;

    void resetDidComplete();
    void resetRequested();
    void setCompletions(std::initializer_list<std::string> newCompl);

  protected:
    GuiTextField textField;
    bool hasTargetBlock;
    bool didComplete;
    bool requestedCompletions;
    int completionIdx;
    std::vector<std::string> completions;

  private:
    void requestCompletions(std::string prefix);
};
