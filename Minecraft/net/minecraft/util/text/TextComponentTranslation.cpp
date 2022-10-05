#include "TextComponentTranslation.h"

#include "../../../../../compile-time-regular-expressions/single-header/ctre.hpp"
#include "TextComponentString.h"

#include <fmt/core.h>

static constexpr auto STRING_VARIABLE_PATTERN = ctll::fixed_string{"(%(?:(\d+)\$)?([A-Za-z%]|$))"};

std::string TextComponentTranslation::getKey() const
{
    return key;
}

void TextComponentTranslation::initializeFromFormat(std::string_view format)
{
    bool flag    = false;
    auto matcher = ctre::match<STRING_VARIABLE_PATTERN>(format);
    size_t i     = 0;
    size_t j     = 0;

    try
    {
        size_t l;
        for (; matcher.find(j); j = l)
        {
            int k = matcher.start();
            l     = matcher.end();
            if (k > j)
            {
                auto textcomponentstring = std::make_unique<TextComponentString>(fmt::format(format.substring(j, k)));
                textcomponentstring.getStyle().setParentStyle(getStyle());
                children.add(textcomponentstring);
            }

            String s2 = matcher.group(2);
            String s  = format.substring(k, l);
            if ("%".equals(s2) && "%%".equals(s))
            {
                TextComponentString textcomponentstring2 = new TextComponentString("%");
                textcomponentstring2.getStyle().setParentStyle(this.getStyle());
                children.add(textcomponentstring2);
            }
            else
            {
                if (!"s".equals(s2))
                {
                    throw new TextComponentTranslationFormatException(this, "Unsupported format: '" + s + "'");
                }

                String s1 = matcher.group(1);
                int i1    = s1 != nullptr ? Integer.parseInt(s1) - 1 : i++;
                if (i1 < this.formatArgs.length)
                {
                    children.add(this.getFormatArgumentAsComponent(i1));
                }
            }
        }

        if (j < format.length())
        {
            TextComponentString textcomponentstring1 = new TextComponentString(String.format(format.substring(j)));
            textcomponentstring1.getStyle().setParentStyle(this.getStyle());
            children.add(textcomponentstring1);
        }
    }
    catch (IllegalFormatException var12)
    {
        throw new TextComponentTranslationFormatException(this, var12);
    }
}

ITextComponent *TextComponentTranslation::getFormatArgumentAsComponent(uint64_t index)
{
    if (index >= formatArgs.length)
    {
        throw new TextComponentTranslationFormatException(this, index);
    }
    else
    {
        Object object = formatArgs[index];
        Object itextcomponent;
        if (object instanceof ITextComponent)
        {
            itextcomponent = (ITextComponent)object;
        }
        else
        {
            itextcomponent = new TextComponentString(object == nullptr ? "null" : object.toString());
            ((ITextComponent)itextcomponent).getStyle().setParentStyle(this.getStyle());
        }

        return (ITextComponent)itextcomponent;
    }
}

bool operator==(const TextComponentTranslation &a, const TextComponentTranslation &b)
{
    return a.formatArgs == b.formatArgs && a.key == b.key && TextComponentBase::operator==(a, b);
}
