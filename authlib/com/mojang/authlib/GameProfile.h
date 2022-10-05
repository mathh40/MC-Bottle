#pragma once
#include "../../../../crossguid/include/crossguid/guid.hpp"

class GameProfile
{
  public:
    GameProfile(xg::Guid id, std::string_view name);
    xg::Guid getId() const;
    std::string getName() const;
    std::unordered_map<> getProperties();
    bool isComplete() const;
    friend bool operator==(const GameProfile &lhs, const GameProfile &rhs);
    friend bool operator!=(const GameProfile &lhs, const GameProfile &rhs);
    std::string toString();
    bool isLegacy() const;

  private:
    xg::Guid id;
    std::string name;
    std::unordered_map<> properties;
    bool legacy;
};

namespace std
{
template <> class hash<GameProfile>
{
  public:
    size_t operator()(const GameProfile &s) const noexcept
    {
        auto result = s.getId().isValid() ? std::hash<xg::Guid>{}(s.getId()) : 0;
        result      = 31 * result + (!(s.getName().empty()) ? std::hash<std::string>{}(s.getName()) : 0);
        return result;
    }
};
} // namespace std
