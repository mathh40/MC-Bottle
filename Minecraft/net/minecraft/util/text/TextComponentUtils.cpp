#include "TextComponentUtils.h"

#include "../../entity/Entity.h"
#include "TextComponentScore.h"
#include "TextComponentString.h"
#include "TextComponentTranslation.h"

ITextComponent *TextComponentUtils::processComponent(ICommandSender *commandSender, ITextComponent *component,
                                                     Entity *entityIn)
{
    Object itextcomponent;
    if (Util:: instanceof <TextComponentScore>(component))
    {
        TextComponentScore *textcomponentscore = (TextComponentScore *)component;
        std::string s                          = textcomponentscore->getName();
        if (EntitySelector.isSelector(s))
        {
            List list = EntitySelector.matchEntities(commandSender, s, entity->class);
            if (list.size() != 1)
            {
                throw new EntityNotFoundException("commands.generic.selector.notFound", new Object[]{s});
            }

            Entity *entity = (Entity)list.get(0);
            if (entity instanceof EntityPlayer)
            {
                s = entity->getName();
            }
            else
            {
                s = entity->getCachedUniqueIdString();
            }
        }

        std::string s2 = entityIn != nullptr && s == "*" ? entityIn->getName() : s;
        itextcomponent = new TextComponentScore(s2, textcomponentscore->getObjective());
        ((TextComponentScore *)itextcomponent).setValue(textcomponentscore->getUnformattedComponentText());
        ((TextComponentScore *)itextcomponent).resolve(commandSender);
    }
    else if (component instanceof TextComponentSelector)
    {
        std::string s1 = ((TextComponentSelector *)component)->getSelector();
        itextcomponent = EntitySelector.matchEntitiesToTextComponent(commandSender, s1);
        if (itextcomponent == nullptr)
        {
            itextcomponent = new TextComponentString("");
        }
    }
    else if (component instanceof TextComponentString)
    {
        itextcomponent = new TextComponentString(((TextComponentString *)component)->getText());
    }
    else if (component instanceof TextComponentKeybind)
    {
        itextcomponent = new TextComponentKeybind(((TextComponentKeybind *)component)->getKeybind());
    }
    else
    {
        if (!(component instanceof TextComponentTranslation))
        {
            return component;
        }

        auto aobject = ((TextComponentTranslation *)component)->getFormatArgs();

        for (int i = 0; i < aobject.length; ++i)
        {
            auto object = aobject[i];
            if (object instanceof ITextComponent)
            {
                aobject[i] = processComponent(commandSender, (ITextComponent)object, entityIn);
            }
        }

        itextcomponent = new TextComponentTranslation(((TextComponentTranslation)component).getKey(), aobject);
    }

    Style style = component.getStyle();
    if (style != null)
    {
        ((ITextComponent)itextcomponent).setStyle(style.createShallowCopy());
    }

    Iterator var15 = component.getSiblings().iterator();

    while (var15.hasNext())
    {
        ITextComponent itextcomponent1 = (ITextComponent)var15.next();
        ((ITextComponent)itextcomponent).appendSibling(processComponent(commandSender, itextcomponent1, entityIn));
    }

    return (ITextComponent)itextcomponent;
}
