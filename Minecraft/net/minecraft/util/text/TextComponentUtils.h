#include "ITextComponent.h"

class ICommandSender;
class Entity;

namespace TextComponentUtils
{
	ITextComponent* processComponent(ICommandSender* commandSender, ITextComponent* component, Entity* entityIn);
}
