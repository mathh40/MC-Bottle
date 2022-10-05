#pragma once
#include "../../../../compile-time-regular-expressions/single-header/ctre.hpp"
#include "ICommandSender.h"
#include "Util.h"
#include "math/AxisAlignedBB.h"
#include "math/MathHelper.h"

#include <functional>
#include <set>
#include <string_view>
#include <vector>
class ITextComponent;
class Entity;
class EntityPlayerMP;

namespace EntitySelector
{

std::set<std::string_view> VALID_ARGUMENTS;

std::string_view addArgument(std::string_view argument)
{
    VALID_ARGUMENTS.emplace(argument);
    return argument;
}

std::string_view ARGUMENT_RANGE_MAX    = addArgument("r");
std::string_view ARGUMENT_RANGE_MIN    = addArgument("rm");
std::string_view ARGUMENT_LEVEL_MAX    = addArgument("l");
std::string_view ARGUMENT_LEVEL_MIN    = addArgument("lm");
std::string_view ARGUMENT_COORDINATE_X = addArgument("x");
std::string_view ARGUMENT_COORDINATE_Y = addArgument("y");
std::string_view ARGUMENT_COORDINATE_Z = addArgument("z");
std::string_view ARGUMENT_DELTA_X      = addArgument("dx");
std::string_view ARGUMENT_DELTA_Y      = addArgument("dy");
std::string_view ARGUMENT_DELTA_Z      = addArgument("dz");
std::string_view ARGUMENT_ROTX_MAX     = addArgument("rx");
std::string_view ARGUMENT_ROTX_MIN     = addArgument("rxm");
std::string_view ARGUMENT_ROTY_MAX     = addArgument("ry");
std::string_view ARGUMENT_ROTY_MIN     = addArgument("rym");
std::string_view ARGUMENT_COUNT        = addArgument("c");
std::string_view ARGUMENT_MODE         = addArgument("m");
std::string_view ARGUMENT_TEAM_NAME    = addArgument("team");
std::string_view ARGUMENT_PLAYER_NAME  = addArgument("name");
std::string_view ARGUMENT_ENTITY_TYPE  = addArgument("type");
std::string_view ARGUMENT_ENTITY_TAG   = addArgument("tag");
constexpr auto IS_VALID_ARGUMENT       = [](std::string_view p_apply_1_) {
    constexpr std::string_view score = "score_";
    return !p_apply_1_.empty() && (EntitySelector::VALID_ARGUMENTS.contains(p_apply_1_) ||
                                   p_apply_1_.size() > score.size() && Util::startwith(score, p_apply_1_));
};
constexpr std::set<std::string_view> WORLD_BINDING_ARGS = {
    ARGUMENT_COORDINATE_X, ARGUMENT_COORDINATE_Y, ARGUMENT_COORDINATE_Z, ARGUMENT_DELTA_X,
    ARGUMENT_DELTA_Y,      ARGUMENT_DELTA_Z,      ARGUMENT_RANGE_MIN,    ARGUMENT_RANGE_MAX};

EntityPlayerMP *matchOnePlayer(ICommandSender *sender, std::string_view token);
std::vector<EntityPlayerMP *> getPlayers(ICommandSender *sender, std::string_view token);
template <class T> Entity *matchOneEntity(ICommandSender *sender, std::string_view token)
{
    std::vector<Entity *> list = matchEntities<T>(sender, token);
    return list.size() == 1 ? list[0] : nullptr;
}
ITextComponent *matchEntitiesToTextComponent(ICommandSender *sender, std::string_view token);
std::vector<World *> getWorlds(ICommandSender *sender,
                               std::unordered_map<std::string_view, std::string_view> &argumentMap);
bool isEntityTypeValid(ICommandSender *commandSender, std::unordered_map<std::string_view, std::string_view> &params);
std::vector<std::function<bool(Entity *p_apply_1_)>> getTypePredicates(
    std::unordered_map<std::string_view, std::string_view> &params, std::string_view type);
std::vector<std::function<bool(Entity *p_apply_1_)>> getXpLevelPredicates(
    std::unordered_map<std::string_view, std::string_view> &params);
std::vector<std::function<bool(Entity *p_apply_1_)>> getGamemodePredicates(
    std::unordered_map<std::string_view, std::string_view> &params);
std::vector<std::function<bool(Entity *p_apply_1_)>> getTeamPredicates(
    std::unordered_map<std::string_view, std::string_view> &params);
std::vector<std::function<bool(Entity *p_apply_1_)>> getScorePredicates(
    ICommandSender *sender, std::unordered_map<std::string_view, std::string_view> &params);
std::vector<std::function<bool(Entity *p_apply_1_)>> getNamePredicates(
    std::unordered_map<std::string_view, std::string_view> &params);
std::vector<std::function<bool(Entity *p_apply_1_)>> getTagPredicates(
    std::unordered_map<std::string_view, std::string_view> &params);
std::vector<std::function<bool(Entity *p_apply_1_)>> getRadiusPredicates(
    std::unordered_map<std::string_view, std::string_view> &params, Vec3d pos);
std::vector<std::function<bool(Entity *p_apply_1_)>> getRotationsPredicates(
    std::unordered_map<std::string_view, std::string_view> &params);
std::unordered_map<std::string_view, std::string_view> getArgumentMap(std::string_view argumentString);
AxisAlignedBB getAABB(BlockPos pos, int32_t x, int32_t y, int32_t z);
BlockPos getBlockPosFromArguments(std::unordered_map<std::string_view, std::string_view> &params, const BlockPos &pos);
Vec3d getPosFromArguments(std::unordered_map<std::string_view, std::string_view> &params, const Vec3d &pos);
double getCoordinate(std::unordered_map<std::string_view, std::string_view> &params, std::string_view key,
                     double defaultD, bool offset);
bool hasArgument(std::unordered_map<std::string_view, std::string_view> &params);
int32_t getInt(std::unordered_map<std::string_view, std::string_view> &params, std::string_view key, int32_t defaultI);
std::string_view getArgument(std::unordered_map<std::string_view, std::string_view> &params, std::string_view key);
std::unordered_map<std::string_view, std::string_view> getScoreMap(
    std::unordered_map<std::string_view, std::string_view> &params);
bool matchesMultiplePlayers(std::string_view selectorStr);
bool isSelector(std::string_view selectorStr);

template <class entityClass>
std::vector<entityClass *> filterResults(std::unordered_map<std::string_view, std::string_view> &params,
                                         std::vector<std::function<bool(Entity *p_apply_1_)>> &inputList,
                                         std::string_view type, World *worldIn, const BlockPos &position)
{
    {
        std::vector<entityClass *> list;
        auto s         = getArgument(params, ARGUMENT_ENTITY_TYPE);
        s              = !(s.empty()) && Util::startwith("!", s) ? s.substr(1) : s;
        bool flag      = !(type == "e");
        bool flag1     = type == ("r") && !s.empty();
        auto i         = getInt(params, ARGUMENT_DELTA_X, 0);
        auto j         = getInt(params, ARGUMENT_DELTA_Y, 0);
        auto k         = getInt(params, ARGUMENT_DELTA_Z, 0);
        auto l         = getInt(params, ARGUMENT_RANGE_MAX, -1);
        auto predicate = inputList;
        predicate.emplace_back(EntitySelectors::IS_ALIVE);

        AxisAlignedBB axisalignedbb1;
        if (!params.contains(ARGUMENT_DELTA_X) && !params.contains(ARGUMENT_DELTA_Y) &&
            !params.contains(ARGUMENT_DELTA_Z))
        {
            if (l >= 0)
            {
                axisalignedbb1 = AxisAlignedBB((double)(position.getx() - l), (double)(position.gety() - l),
                                               (double)(position.getz() - l), (double)(position.getx() + l + 1),
                                               (double)(position.gety() + l + 1), (double)(position.getz() + l + 1));
                if (flag && !flag1)
                {
                    list.assign(worldIn->getPlayers<entityClass>(predicate1));
                }
                else
                {
                    list.assign(worldIn->getEntitiesWithinAABB<entityClass>(axisalignedbb1, predicate1));
                }
            }
            else if (type == "a")
            {
                list.assign(worldIn->getPlayers<entityClass>(predicate));
            }
            else if (type == "p" || type == "r" && !flag1)
            {
                list.assign(worldIn->getPlayers<entityClass>(predicate1));
            }
            else
            {
                list.assign(worldIn->getEntities<entityClass>(predicate1));
            }
        }
        else
        {
            axisalignedbb1 = getAABB(position, i, j, k);
            if (flag && !flag1)
            {
                auto predicate2 = [&](Entity *p_apply_1_) {
                    return p_apply_1_ != null && axisalignedbb1.intersects(p_apply_1_->getEntityBoundingBox());
                };
                list.assign(worldIn->getPlayers<entityClass>((predicate1, predicate2)));
            }
            else
            {
                list.assign(worldIn->getEntitiesWithinAABB<entityClass>(axisalignedbb1, predicate1));
            }
        }

        return list;
    }
}

std::vector<Entity *> matchEntities(ICommandSender *sender, std::string_view token)
{
    static constexpr auto NAME_PATTERN = ctll::fixed_string{"^@([pares])(?:\\[([^ ]*)\\])?$"};
    if (auto matcher = ctre::match<NAME_PATTERN>(token) && sender->canUseCommand(1, "@"))
    {
        auto map = getArgumentMap(matcher.get<2>().to_view());
        if (!isEntityTypeValid(sender, map))
        {
            return {};
        }
        else
        {
            auto s            = matcher.get<1>().to_view();
            BlockPos blockpos = getBlockPosFromArguments(map, sender->getPosition());
            Vec3d vec3d       = getPosFromArguments(map, sender->getPositionVector());
            auto list         = getWorlds(sender, map);
            List list1        = Lists.newArrayList();

            for (auto world : list)
            {
                if (world != nullptr)
                {
                    std::vector<std::function<bool(Entity * p_apply_1_)>> list2;
                    list2 += (getTypePredicates(map, s));
                    list2 += (getXpLevelPredicates(map));
                    list2 += (getGamemodePredicates(map));
                    list2 += (getTeamPredicates(map));
                    list2 += (getScorePredicates(sender, map));
                    list2 += (getNamePredicates(map));
                    list2 += (getTagPredicates(map));
                    list2 += (getRadiusPredicates(map, vec3d));
                    list2 += (getRotationsPredicates(map));
                    if ("s".equalsIgnoreCase(s))
                    {
                        Entity *entity = sender->getCommandSenderEntity();
                        if (entity != nullptr && targetClass.isAssignableFrom(entity.getClass()))
                        {
                            if (map.containsKey(ARGUMENT_DELTA_X) || map.containsKey(ARGUMENT_DELTA_Y) ||
                                map.containsKey(ARGUMENT_DELTA_Z))
                            {
                                int i                       = getInt(map, ARGUMENT_DELTA_X, 0);
                                int j                       = getInt(map, ARGUMENT_DELTA_Y, 0);
                                int k                       = getInt(map, ARGUMENT_DELTA_Z, 0);
                                AxisAlignedBB axisalignedbb = getAABB(blockpos, i, j, k);
                                if (!axisalignedbb.intersects(entity.getEntityBoundingBox()))
                                {
                                    return {};
                                }
                            }

                            Iterator var18 = list2.iterator();

                            Predicate predicate;
                            do
                            {
                                if (!var18.hasNext())
                                {
                                    return Lists.newArrayList(new Entity[]{entity});
                                }

                                predicate = (Predicate)var18.next();
                            } while (predicate.apply(entity));

                            return Collections.emptyList();
                        }

                        return Collections.emptyList();
                    }

                    list1.addAll(filterResults(map, targetClass, list2, s, world, blockpos));
                }
            }

            return getEntitiesFromPredicates(list1, map, sender, targetClass, s, vec3d);
        }
    }
    else
    {
        return Collections.emptyList();
    }
}

template <class T>
std::vector<T *> getEntitiesFromPredicates(std::vector<T *> matchingEntities, Map params, ICommandSender *sender,
                                           T targetClass, std::string_view type, Vec3d pos)
{
    int i = getInt(params, ARGUMENT_COUNT, !type.equals("a") && !type.equals("e") ? 1 : 0);
    if (!type.equals("p") && !type.equals("a") && !type.equals("e"))
    {
        if (type.equals("r"))
        {
            Collections.shuffle((List)matchingEntities);
        }
    }
    else
    {
         Collections.sort((List)matchingEntities, new Comparator() {
            public int compare(Entity p_compare_1_, Entity p_compare_2_) {
               return ComparisonChain.start().compare(p_compare_1_.getDistanceSq(pos.x, pos.y, pos.z), p_compare_2_.getDistanceSq(pos.x, pos.y, pos.z)).result();
    }
});
} // namespace EntitySelector

Entity entity = sender.getCommandSenderEntity();
if (entity != null && targetClass.isAssignableFrom(entity.getClass()) && i == 1 &&
    ((List)matchingEntities).contains(entity) && !"r".equals(type))
{
    matchingEntities = Lists.newArrayList(new Entity[]{entity});
}

if (i != 0)
{
    if (i < 0)
    {
        Collections.reverse((List)matchingEntities);
    }

    matchingEntities = ((List)matchingEntities).subList(0, Math.min(Math.abs(i), ((List)matchingEntities).size()));
}

return (List)matchingEntities;
}
}
