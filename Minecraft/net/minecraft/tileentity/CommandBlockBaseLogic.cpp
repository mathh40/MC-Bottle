#include "CommandBlockBaseLogic.h"
#include "text\TextComponentString.h"
#include "ReportedException.h"

int32_t CommandBlockBaseLogic::getSuccessCount()
{
	return successCount;
}

void CommandBlockBaseLogic::setSuccessCount(int32_t successCountIn)
{
	successCount = successCountIn;
}

ITextComponent* CommandBlockBaseLogic::getLastOutput()
{
	return (lastOutput == nullptr ? std::make_unique<TextComponentString>("") : lastOutput;
}

NBTTagCompound* CommandBlockBaseLogic::writeToNBT(NBTTagCompound* p_189510_1_)
{
	p_189510_1_->setString("Command", commandStored);
	p_189510_1_->setInteger("SuccessCount", successCount);
	p_189510_1_->setString("CustomName", customName);
	p_189510_1_->setBoolean("TrackOutput", trackOutput);
	if (lastOutput != nullptr && trackOutput) {
		p_189510_1_->setString("LastOutput", ITextComponent::Serializer.componentToJson(lastOutput));
	}

	p_189510_1_->setBoolean("UpdateLastExecution", updateLastExecution);
	if (updateLastExecution && lastExecution > 0L) {
		p_189510_1_->setLong("LastExecution", lastExecution);
	}

	resultStats.writeStatsToNBT(p_189510_1_);
	return p_189510_1_;
}

void CommandBlockBaseLogic::readDataFromNBT(NBTTagCompound* nbt)
{
	commandStored = nbt->getString("Command");
	successCount = nbt->getInteger("SuccessCount");
	if (nbt->hasKey("CustomName", 8)) {
		customName = nbt->getString("CustomName");
	}

	if (nbt->hasKey("TrackOutput", 1)) {
		trackOutput = nbt->getBoolean("TrackOutput");
	}

	if (nbt->hasKey("LastOutput", 8) && trackOutput) {
		try {
		lastOutput = ITextComponent.Serializer.jsonToComponent(nbt->getString("LastOutput"));
		} catch (Throwable var3) {
		lastOutput = std::make_unique<TextComponentString>(var3.getMessage());
		}
	} else {
		lastOutput = nullptr;
	}

	if (nbt->hasKey("UpdateLastExecution")) {
		updateLastExecution = nbt->getBoolean("UpdateLastExecution");
	}

	if (updateLastExecution && nbt->hasKey("LastExecution")) {
		lastExecution = nbt->getLong("LastExecution");
	} else {
		lastExecution = -1L;
	}

	resultStats.readStatsFromNBT(nbt);
}

void CommandBlockBaseLogic::setCommand(std::string_view command)
{
	commandStored = command;
	successCount = 0;
}

std::string CommandBlockBaseLogic::getCommand() const
{
	return commandStored;
}

bool CommandBlockBaseLogic::trigger(World* worldIn)
{
	if (!worldIn->isRemote && worldIn->getTotalWorldTime() != lastExecution) 
	{
		if ("Searge".equalsIgnoreCase(commandStored)) 
		{
			lastOutput = std::make_unique<TextComponentString>("#itzlipofutzli");
			successCount = 1;
			return true;
		}
		else 
		{
			MinecraftServer* minecraftserver = getServer();
			if (minecraftserver != nullptr && minecraftserver.isAnvilFileSet() && minecraftserver.isCommandBlockEnabled()) 
			{
				try 
				{
					lastOutput = nullptr;
					successCount = minecraftserver.getCommandManager().executeCommand(this, commandStored);
				}
				catch (std::exception& var6) 
				{
					CrashReport crashreport = CrashReport.makeCrashReport(var6, "Executing command block");
					CrashReportCategory crashreportcategory = crashreport.makeCategory("Command to be executed");
					crashreportcategory.addDetail("Command", new ICrashReportDetail() {
						public String call() throws Exception {
							return CommandBlockBaseLogic.this.getCommand();
						}
					});
					crashreportcategory.addDetail("Name", new ICrashReportDetail() {
						public String call() throws Exception {
							return CommandBlockBaseLogic.this.getName();
						}
					});
					throw new ReportedException(crashreport);
				}
			}
			else 
			{
				successCount = 0;
			}

			if (updateLastExecution) 
			{
				lastExecution = worldIn->getTotalWorldTime();
			}
			else 
			{
				lastExecution = -1L;
			}

			return true;
		}
	}
	else 
	{
		return false;
	}
}

std::string CommandBlockBaseLogic::getName() const
{
	return customName;
}

void CommandBlockBaseLogic::setName(std::string_view name)
{
	customName = name;
}

void CommandBlockBaseLogic::sendMessage(ITextComponent* component)
{
	if (trackOutput && getEntityWorld() != nullptr && !getEntityWorld().isRemote) 
	{
		lastOutput = (new TextComponentString("[" + TIMESTAMP_FORMAT.format(new Date()) + "] ")).appendSibling(component);
		updateCommand();
	}
}

bool CommandBlockBaseLogic::sendCommandFeedback()
{
	MinecraftServer minecraftserver = getServer();
	return minecraftserver == nullptr || !minecraftserver.isAnvilFileSet() || minecraftserver.worlds[0].getGameRules().getBoolean("commandBlockOutput");
}

void CommandBlockBaseLogic::setCommandStat(CommandResultStats::Type type, int32_t amount)
{
	resultStats.setCommandStatForSender(getServer(), this, type, amount);
}

void CommandBlockBaseLogic::setLastOutput(ITextComponent* lastOutputMessage)
{
	lastOutput = lastOutputMessage;
}

void CommandBlockBaseLogic::setTrackOutput(bool shouldTrackOutput)
{
	trackOutput = shouldTrackOutput;
}

bool CommandBlockBaseLogic::shouldTrackOutput() const
{
	return trackOutput;
}

bool CommandBlockBaseLogic::tryOpenEditCommandBlock(EntityPlayer* playerIn)
{
	if (!playerIn->canUseCommandBlock()) 
	{
		return false;
	}
	else 
	{
		if (playerIn->getEntityWorld().isRemote) 
		{
			playerIn->displayGuiEditCommandCart(this);
		}

		return true;
	}
}

CommandResultStats CommandBlockBaseLogic::getCommandResultStats()
{
	return resultStats;
}
