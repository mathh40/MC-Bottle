#include "TextComponentUtils.h"
#include "TextComponentScore.h"

ITextComponent TextComponentUtils::processComponent(ICommandSender commandSender, ITextComponent component, Entity entityIn)
{
	Object itextcomponent;
	if (component instanceof TextComponentScore) {
		TextComponentScore textcomponentscore = (TextComponentScore)component;
		String s = textcomponentscore.getName();
		if (EntitySelector.isSelector(s)) {
			List list = EntitySelector.matchEntities(commandSender, s, entity->class);
			if (list.size() != 1) {
				throw new EntityNotFoundException("commands.generic.selector.notFound", new Object[]{ s });
			}

			Entity entity = (Entity)list.get(0);
			if (entity instanceof EntityPlayer) {
				s = entity->getName();
			}
			else {
				s = entity->getCachedUniqueIdString();
			}
		}

		String s2 = entityIn != null && s.equals("*") ? entityIn.getName() : s;
		itextcomponent = new TextComponentScore(s2, textcomponentscore.getObjective());
		((TextComponentScore)itextcomponent).setValue(textcomponentscore.getUnformattedComponentText());
		((TextComponentScore)itextcomponent).resolve(commandSender);
	}
	else if (component instanceof TextComponentSelector) {
		String s1 = ((TextComponentSelector)component).getSelector();
		itextcomponent = EntitySelector.matchEntitiesToTextComponent(commandSender, s1);
		if (itextcomponent == null) {
			itextcomponent = new TextComponentString("");
		}
	}
	else if (component instanceof TextComponentString) {
		itextcomponent = new TextComponentString(((TextComponentString)component).getText());
	}
	else if (component instanceof TextComponentKeybind) {
		itextcomponent = new TextComponentKeybind(((TextComponentKeybind)component).getKeybind());
	}
	else {
		if (!(component instanceof TextComponentTranslation)) {
			return component;
		}

		Object[] aobject = ((TextComponentTranslation)component).getFormatArgs();

		for (int i = 0; i < aobject.length; ++i) {
			Object object = aobject[i];
			if (object instanceof ITextComponent) {
				aobject[i] = processComponent(commandSender, (ITextComponent)object, entityIn);
			}
		}

		itextcomponent = new TextComponentTranslation(((TextComponentTranslation)component).getKey(), aobject);
	}

	Style style = component.getStyle();
	if (style != null) {
		((ITextComponent)itextcomponent).setStyle(style.createShallowCopy());
	}

	Iterator var15 = component.getSiblings().iterator();

	while (var15.hasNext()) {
		ITextComponent itextcomponent1 = (ITextComponent)var15.next();
		((ITextComponent)itextcomponent).appendSibling(processComponent(commandSender, itextcomponent1, entityIn));
	}

	return (ITextComponent)itextcomponent;
}
