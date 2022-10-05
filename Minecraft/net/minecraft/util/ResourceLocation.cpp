#include "ResourceLocation.h"

void ResourceLocation::generate(std::optional<std::string> resourceloc, std::string resourceName)
{
    std::locale loc;
    if (resourceloc.has_value())
    {
        std::transform(resourceloc.value.begin(), resourceloc.value.end(), resourceloc.value.begin(),
                       [&](const char c) { return std::tolower(c, loc); });
        name_space = resourceloc.value;
    }
    else
        name_space = "minecraft";

    std::transform(resourceName.begin(), resourceName.end(), resourceName.begin(),
                   [&](const char c) { return std::tolower(c, loc); });
    path = resourceName;
}

ResourceLocation::ResourceLocation(std::string_view resourceName)
{
    auto [name_space, path] = splitObjectName(resourceName);
    generate(name_space, path);
}

std::string ResourceLocation::getPath() const
{
    return path.string;
}

std::string ResourceLocation::getNamespace() const
{
    return name_space;
}

std::string ResourceLocation::to_string() const
{
    return name_space + ':' + path.string();
}

int ResourceLocation::compareTo(ResourceLocation p_compareTo_1_) const
{
    auto i = name_space.compare(p_compareTo_1_.name_space);
    if (i == 0)
    {
        i = path.compare(p_compareTo_1_.path);
    }

    return i;
}

bool operator==(const ResourceLocation &a, const ResourceLocation &b)
{
    return a.getNamespace() == b.getNamespace() && a.getPath() == (b.getPath());
}

ResourceLocation::ResourceLocation(std::string_view namespaceIn, std::string_view pathIn)
{
    generate(namespaceIn, pathIn);
}

std::tuple<std::string, std::string> ResourceLocation::splitObjectName(std::string_view toSplit)
{
    std::string name_space = "minecraft";
    std::string path;
    auto i = toSplit.find(':');
    path   = toSplit.substr(i + 1, toSplit.length());
    if (i > 1)
    {
        name_space = toSplit.substr(0, i);
    }

    return {name_space, path};
}
