#include "EntitySelector.h"

#include "../entity/Entity.h"
#include "../entity/EntityList.h"
#include "CommandException.h"
#include "Util.h"

#include <set>

namespace EntitySelector
{

EntityPlayerMP *matchOnePlayer(ICommandSender *sender, std::string_view token)
{
    return matchOneEntity<EntityPlayerMP>(sender, token);
}

std::vector<EntityPlayerMP *> getPlayers(ICommandSender *sender, std::string_view token)
{
    return matchEntities<EntityPlayerMP>(sender, token);
}

ITextComponent *matchEntitiesToTextComponent(ICommandSender *sender, std::string_view token)
{
    auto list = matchEntities<Entity>(sender, token);
    if (list.empty())
    {
        return nullptr;
    }
    else
    {
        std::vector<ITextComponent *> list1;

        for (auto entity : list)
        {
            list1.emplace_back(entity->getDisplayName());
        }

        return CommandBase.join(list1);
    }
}

std::vector<World *> getWorlds(ICommandSender *sender,
                               std::unordered_map<std::string_view, std::string_view> &argumentMap)
{
    std::vector<World *> list;
    if (hasArgument(argumentMap))
    {
        list.emplace_back(sender->getEntityWorld());
    }
    else
    {
        Collections.addAll(list, sender->getServer()->worlds);
    }

    return list;
}

bool isEntityTypeValid(ICommandSender *commandSender, std::unordered_map<std::string_view, std::string_view> &params)
{
    auto s = getArgument(params, ARGUMENT_ENTITY_TYPE);
    if (s == nullptr)
    {
        return true;
    }
    else
    {
        ResourceLocation resourcelocation = ResourceLocation(s.startsWith("!") ? s.substring(1) : s);
        if (EntityList::isRegistered(resourcelocation))
        {
            return true;
        }
        else
        {
            TextComponentTranslation textcomponenttranslation =
                new TextComponentTranslation("commands.generic.entity.invalidType", {resourcelocation});
            textcomponenttranslation.getStyle().setColor(TextFormatting::RED);
            commandSender->sendMessage(textcomponenttranslation);
            return false;
        }
    }
}

std::vector<std::function<bool(Entity *p_apply_1_)>> getTypePredicates(
    std::unordered_map<std::string_view, std::string_view> &params, std::string_view type)
{
    auto s = getArgument(params, ARGUMENT_ENTITY_TYPE);
    if (s == null || !type.equals("e") && !type.equals("r") && !type.equals("s"))
    {
        return !type.equals("e") && !type.equals("s") ? Collections.singletonList(new Predicate() {
          public
            boolean apply(@Nullable Entity p_apply_1_)
            {
                return p_apply_1_ instanceof EntityPlayer;
            }
        })
                                                      : Collections.emptyList();
    }
    else
    {
        bool flag                         = s.startsWith("!");
        ResourceLocation resourcelocation = ResourceLocation(flag ? s.substring(1) : s);
        return Collections.singletonList(new Predicate() {
          public
            bool apply(@Nullable Entity p_apply_1_)
            {
                return EntityList::isMatchingName(p_apply_1_, resourcelocation) != flag;
            }
        });
    }
}
std::vector<std::function<bool(Entity *p_apply_1_)>> getXpLevelPredicates(
    std::unordered_map<std::string_view, std::string_view> &params)
{
    List list   = Lists.newArrayList();
    final int i = getInt(params, ARGUMENT_LEVEL_MIN, -1);
    final int j = getInt(params, ARGUMENT_LEVEL_MAX, -1);
    if (i > -1 || j > -1)
    {
        list.add(new Predicate() {
          public
            bool apply(@Nullable Entity p_apply_1_)
            {
                if (!(p_apply_1_ instanceof EntityPlayerMP))
                {
                    return false;
                }
                else
                {
                    EntityPlayerMP entityplayermp = (EntityPlayerMP)p_apply_1_;
                    return (i <= -1 || entityplayermp.experienceLevel >= i) &&
                           (j <= -1 || entityplayermp.experienceLevel <= j);
                }
            }
        });
    }

    return list;
}

std::vector<std::function<bool(Entity *p_apply_1_)>> getGamemodePredicates(
    std::unordered_map<std::string_view, std::string_view> &params)
{
    List list = Lists.newArrayList();
    String s  = getArgument(params, ARGUMENT_MODE);
    if (s == null)
    {
        return list;
    }
    else
    {
        final boolean flag = s.startsWith("!");
        if (flag)
        {
            s = s.substring(1);
        }

        final GameType gametype;
        try
        {
            int i    = Integer.parseInt(s);
            gametype = GameType.parseGameTypeWithDefault(i, GameType.NOT_SET);
        }
        catch (Throwable var6)
        {
            gametype = GameType.parseGameTypeWithDefault(s, GameType.NOT_SET);
        }

        list.add(new Predicate() {
          public
            boolean apply(@Nullable Entity p_apply_1_)
            {
                if (!(p_apply_1_ instanceof EntityPlayerMP))
                {
                    return false;
                }
                else
                {
                    EntityPlayerMP entityplayermp = (EntityPlayerMP)p_apply_1_;
                    GameType gametype1            = entityplayermp.interactionManager.getGameType();
                    return flag ? gametype1 != gametype : gametype1 == gametype;
                }
            }
        });
        return list;
    }
}

std::vector<std::function<bool(Entity *p_apply_1_)>> getTeamPredicates(
    std::unordered_map<std::string_view, std::string_view> &params)
{
    List list          = Lists.newArrayList();
    final String s     = getArgument(params, ARGUMENT_TEAM_NAME);
    final boolean flag = s != null && s.startsWith("!");
    if (flag)
    {
        s = s.substring(1);
    }

    if (s != null)
    {
        list.add(new Predicate() {
          public
            boolean apply(@Nullable Entity p_apply_1_)
            {
                if (!(p_apply_1_ instanceof EntityLivingBase))
                {
                    return false;
                }
                else
                {
                    EntityLivingBase entitylivingbase = (EntityLivingBase)p_apply_1_;
                    Team team                         = entitylivingbase.getTeam();
                    String s1                         = team == null ? "" : team.getName();
                    return s1.equals(s) != flag;
                }
            }
        });
    }

    return list;
}

std::vector<std::function<bool(Entity *p_apply_1_)>> getScorePredicates(
    ICommandSender *sender, std::unordered_map<std::string_view, std::string_view> &params)
{
    final Map map = getScoreMap(params);
      return (List)(map.isEmpty() ? Collections.emptyList() : Lists.newArrayList(new Predicate[]{new Predicate() {
         public boolean apply(@Nullable Entity p_apply_1_) {
            if (p_apply_1_ == null) {
               return false;
}
else
{
    Scoreboard scoreboard = sender.getServer().getWorld(0).getScoreboard();
    Iterator var3         = map.entrySet().iterator();

    Entry entry;
    boolean flag;
    int i;
    do
    {
        if (!var3.hasNext())
        {
            return true;
        }

        entry    = (Entry)var3.next();
        String s = (String)entry.getKey();
        flag     = false;
        if (s.endsWith("_min") && s.length() > 4)
        {
            flag = true;
            s    = s.substring(0, s.length() - 4);
        }

        ScoreObjective scoreobjective = scoreboard.getObjective(s);
        if (scoreobjective == null)
        {
            return false;
        }

        String s1 = p_apply_1_ instanceof EntityPlayerMP ? p_apply_1_.getName() : p_apply_1_.getCachedUniqueIdString();
        if (!scoreboard.entityHasObjective(s1, scoreobjective))
        {
            return false;
        }

        Score score = scoreboard.getOrCreateScore(s1, scoreobjective);
        i           = score.getScorePoints();
        if (i < (Integer)entry.getValue() && flag)
        {
            return false;
        }
    } while (i <= (Integer)entry.getValue() || flag);

    return false;
}
} // namespace EntitySelector
}
}));
}

std::vector<std::function<bool(Entity *p_apply_1_)>> getNamePredicates(
    std::unordered_map<std::string_view, std::string_view> &params)
{
    List list          = Lists.newArrayList();
    final String s     = getArgument(params, ARGUMENT_PLAYER_NAME);
    final boolean flag = s != null && s.startsWith("!");
    if (flag)
    {
        s = s.substring(1);
    }

    if (s != null)
    {
        list.add(new Predicate() {
          public
            boolean apply(@Nullable Entity p_apply_1_)
            {
                return p_apply_1_ != null && p_apply_1_.getName().equals(s) != flag;
            }
        });
    }

    return list;
}

std::vector<std::function<bool(Entity *p_apply_1_)>> getTagPredicates(
    std::unordered_map<std::string_view, std::string_view> &params)
{
    List list          = Lists.newArrayList();
    final String s     = getArgument(params, ARGUMENT_ENTITY_TAG);
    final boolean flag = s != null && s.startsWith("!");
    if (flag)
    {
        s = s.substring(1);
    }

    if (s != null)
    {
        list.add(new Predicate() {
          public
            boolean apply(@Nullable Entity p_apply_1_)
            {
                if (p_apply_1_ == null)
                {
                    return false;
                }
                else if ("".equals(s))
                {
                    return p_apply_1_.getTags().isEmpty() != flag;
                }
                else
                {
                    return p_apply_1_.getTags().contains(s) != flag;
                }
            }
        });
    }

    return list;
}

std::vector<std::function<bool(Entity *p_apply_1_)>> getRadiusPredicates(
    std::unordered_map<std::string_view, std::string_view> &params, Vec3d pos)
{
    double d0           = (double)getInt(params, ARGUMENT_RANGE_MIN, -1);
    double d1           = (double)getInt(params, ARGUMENT_RANGE_MAX, -1);
    final boolean flag  = d0 < -0.5D;
    final boolean flag1 = d1 < -0.5D;
    if (flag && flag1)
    {
        return Collections.emptyList();
    }
    else
    {
        double d2       = Math.max(d0, 1.0E-4D);
        final double d3 = d2 * d2;
        double d4       = Math.max(d1, 1.0E-4D);
        final double d5 = d4 * d4;
         return Lists.newArrayList(new Predicate[]{new Predicate() {
            public boolean apply(@Nullable Entity p_apply_1_) {
               if (p_apply_1_ == null) {
                  return false;
    }
    else
    {
        double d6 = pos.squareDistanceTo(p_apply_1_.posX, p_apply_1_.posY, p_apply_1_.posZ);
        return (flag || d6 >= d3) && (flag1 || d6 <= d5);
    }
}
}
});
}
}

   std::vector<std::function<bool(Entity* p_apply_1_)>> getRotationsPredicatesstd::unordered_map<std::string_view, std::string_view>& params)
   {
       List list = Lists.newArrayList();
       final int k;
       final int l;
       if (params.containsKey(ARGUMENT_ROTY_MIN) || params.containsKey(ARGUMENT_ROTY_MAX))
       {
           k = MathHelper.wrapDegrees(getInt(params, ARGUMENT_ROTY_MIN, 0));
           l = MathHelper.wrapDegrees(getInt(params, ARGUMENT_ROTY_MAX, 359));
           list.add(new Predicate() {
             public
               boolean apply(@Nullable Entity p_apply_1_)
               {
                   if (p_apply_1_ == null)
                   {
                       return false;
                   }
                   else
                   {
                       int i1 = MathHelper.wrapDegrees(MathHelper.floor(p_apply_1_.rotationYaw));
                       if (k > l)
                       {
                           return i1 >= k || i1 <= l;
                       }
                       else
                       {
                           return i1 >= k && i1 <= l;
                       }
                   }
               }
           });
       }

       if (params.containsKey(ARGUMENT_ROTX_MIN) || params.containsKey(ARGUMENT_ROTX_MAX))
       {
           k = MathHelper.wrapDegrees(getInt(params, ARGUMENT_ROTX_MIN, 0));
           l = MathHelper.wrapDegrees(getInt(params, ARGUMENT_ROTX_MAX, 359));
           list.add(new Predicate() {
             public
               boolean apply(@Nullable Entity p_apply_1_)
               {
                   if (p_apply_1_ == null)
                   {
                       return false;
                   }
                   else
                   {
                       int i1 = MathHelper::wrapDegrees(MathHelper::floor(p_apply_1_.rotationPitch));
                       if (k > l)
                       {
                           return i1 >= k || i1 <= l;
                       }
                       else
                       {
                           return i1 >= k && i1 <= l;
                       }
                   }
               }
           });
       }

       return list;
   }

   std::unordered_map<std::string_view, std::string_view> getArgumentMap(std::string_view argumentString)
   {
       std::unordered_map<std::string_view, std::string_view> map;
       if (argumentString.empty())
       {
           return map;
       }
       else
       {
           auto var2 = Util::split(argumentString, ",");
           for (auto s : var2)
           {
               auto argumentString = Util::split(s, "=");
               auto s1             = argumentString[0];
               auto s2             = argumentString[1];
               if (!IS_VALID_ARGUMENT(s1))
               {
                   throw CommandException("commands.generic.selector_argument", s);
               }

               map.emplace(s1, s2);
           }
       }

       return map;
   }

   AxisAlignedBB getAABB(BlockPos pos, int32_t x, int32_t y, int32_t z)
   {
       const bool flag  = x < 0;
       const bool flag1 = y < 0;
       const bool flag2 = z < 0;
       int32_t i        = pos.getx() + (flag ? x : 0);
       int32_t j        = pos.gety() + (flag1 ? y : 0);
       int32_t k        = pos.getz() + (flag2 ? z : 0);
       int32_t l        = pos.getx() + (flag ? 0 : x) + 1;
       int32_t i1       = pos.gety() + (flag1 ? 0 : y) + 1;
       int32_t j1       = pos.getz() + (flag2 ? 0 : z) + 1;
       return AxisAlignedBB((double)i, (double)j, (double)k, (double)l, (double)i1, (double)j1);
   }

   BlockPos getBlockPosFromArguments(std::unordered_map<std::string_view, std::string_view> &params,
                                     const BlockPos &pos)
   {
       return BlockPos(getInt(params, ARGUMENT_COORDINATE_X, pos.getx()),
                       getInt(params, ARGUMENT_COORDINATE_Y, pos.gety()),
                       getInt(params, ARGUMENT_COORDINATE_Z, pos.getz()));
   }

   Vec3d getPosFromArguments(std::unordered_map<std::string_view, std::string_view> &params, const Vec3d &pos)
   {
       return Vec3d(getCoordinate(params, ARGUMENT_COORDINATE_X, pos.getx(), true),
                    getCoordinate(params, ARGUMENT_COORDINATE_Y, pos.gety(), false),
                    getCoordinate(params, ARGUMENT_COORDINATE_Z, pos.getz(), true));
   }

   double getCoordinate(std::unordered_map<std::string_view, std::string_view> &params, std::string_view key,
                        double defaultD, bool offset)
   {
       return params.contains(key)
                  ? (double)MathHelper::getInt(params[key], MathHelper::floor(defaultD)) + (offset ? 0.5 : 0.0)
                  : defaultD;
   }

   bool hasArgument(std::unordered_map<std::string_view, std::string_view> &params)
   {
       auto ite = WORLD_BINDING_ARGS.begin();

       bool has = false;

       for (auto s : WORLD_BINDING_ARGS)
       {
           has = params.contains(s);
       }

       return has;
   }

   int32_t getInt(std::unordered_map<std::string_view, std::string_view> &params, std::string_view key,
                  int32_t defaultI)
   {
       return params.contains(key) ? MathHelper::getInt(params[key], defaultI) : defaultI;
   }

   std::string_view getArgument(std::unordered_map<std::string_view, std::string_view> &params, std::string_view key)
   {
       return params[key];
   }

   std::unordered_map<std::string_view, std::string_view> getScoreMap(
       std::unordered_map<std::string_view, std::string_view> &params)
   {
       std::unordered_map<std::string_view, std::string_view> map;
       for (auto [s, entry] : params)
       {
           constexpr std::string_view score = "score_";
           if (Util::startwith(score, s) && s.length() > score.size())
           {
               map.emplace(s.substr(score.size()), MathHelper::getInt(entry, 1));
           }
       }

       return map;
   }

   bool matchesMultiplePlayers(std::string_view selectorStr)
   {
       static constexpr auto NAME_PATTERN = ctll::fixed_string{"^@([pares])(?:\\[([^ ]*)\\])?$"};
       auto matcher                       = ctre::match<NAME_PATTERN>(selectorStr);
       if (!matcher)
       {
           return false;
       }
       else
       {
           auto map = getArgumentMap(matcher.get<2>().to_view());
           auto s   = matcher.get<1>().to_view();
           auto i   = !"a" == s && !"e" == s ? 1 : 0;
           return getInt(map, ARGUMENT_COUNT, i) != 1;
       }
   }

   bool isSelector(std::string_view selectorStr)
   {
       static constexpr auto NAME_PATTERN = ctll::fixed_string{"^@([pares])(?:\\[([^ ]*)\\])?$"};
       auto matcher                       = ctre::match<NAME_PATTERN>(selectorStr);
       return matcher;
   }
   }
