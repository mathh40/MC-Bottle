#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <tuple>

class ResourceLocation
{
  protected:
    std::string name_space;
    std::filesystem::path path;
    void generate(std::optional<std::string_view> resourceloc, std::string_view resourceName);
    static std::tuple<std::string, std::string> splitObjectName(std::string_view toSplit);

  public:
    ResourceLocation(std::string_view namespaceIn, std::string_view pathIn);
    explicit ResourceLocation(std::string_view resourceName);

    std::string getPath() const;
    std::string getNamespace() const;
    std::string to_string() const;

    int compareTo(ResourceLocation p_compareTo_1_) const;
};

bool operator==(const ResourceLocation &a, const ResourceLocation &b);

namespace std
{
template <> struct hash<ResourceLocation>
{
    size_t operator()(const ResourceLocation &x) const noexcept
    {
        return 31 * std::hash<std::string>{}(x.getNamespace()) + std::hash<std::string>{}(x.getPath());
    }
};
} // namespace std
