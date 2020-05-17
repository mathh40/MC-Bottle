#include "ShapedRecipes.h"

#include <unordered_set>

#include "Util.h"
#include "../../../../../nlohmann_json/test/thirdparty/doctest/doctest.h"
#include "math/MathHelper.h"

ShapedRecipes::ShapedRecipes(std::string group, int32_t width, int32_t height, std::vector<Ingredient> ingredients,
                             ItemStack result)
        :group(group),recipeWidth(width),recipeHeight(height),recipeItems(ingredients),recipeOutput(result)
{
}

std::string ShapedRecipes::getGroup()
{
    return group;
}

ItemStack ShapedRecipes::getRecipeOutput()
{
    return recipeOutput;
}

std::vector<ItemStack> ShapedRecipes::getRemainingItems(InventoryCrafting *var1)
{
    std::vector<ItemStack> nonnulllist(inv->getSizeInventory(), ItemStack::EMPTY);

    for(int i = 0; i < nonnulllist.size(); ++i) 
    {
        ItemStack itemstack = inv->getStackInSlot(i);
        if (itemstack.getItem().hasContainerItem()) 
        {
            nonnulllist[i] = ItemStack(itemstack.getItem().getContainerItem());
        }
    }

    return nonnulllist;
}

std::vector<Ingredient> ShapedRecipes::getIngredients()
{
    return recipeItems;
}

bool ShapedRecipes::canFit(int32_t width, int32_t height)
{
    return width >= recipeWidth && height >= recipeHeight;
}

bool ShapedRecipes::matches(InventoryCrafting *inv, World *worldIn)
{
    for(auto i = 0; i <= 3 - recipeWidth; ++i) 
    {
        for(auto j = 0; j <= 3 - recipeHeight; ++j) 
        {
            if (checkMatch(inv, i, j, true)) 
            {
                return true;
            }

            if (checkMatch(inv, i, j, false)) 
            {
                return true;
            }
        }
    }

    return false;
}

ItemStack ShapedRecipes::getCraftingResult(InventoryCrafting *inv)
{
    return getRecipeOutput().copy();
}

int32_t ShapedRecipes::getWidth() const
{
    return recipeWidth;
}

int32_t ShapedRecipes::getHeight() const
{
    return recipeHeight;
}

bool ShapedRecipes::checkMatch(InventoryCrafting *craftingInventory, int32_t p_77573_2_, int32_t p_77573_3_,
                               bool p_77573_4_)
{
    for(int i = 0; i < 3; ++i) 
    {
        for(int j = 0; j < 3; ++j) 
        {
            int k = i - p_77573_2_;
            int l = j - p_77573_3_;
            Ingredient ingredient = Ingredient::EMPTY;
            if (k >= 0 && l >= 0 && k < recipeWidth && l < recipeHeight) 
            {
                if (p_77573_4_) 
                {
                    ingredient = recipeItems[recipeWidth - k - 1 + l * recipeWidth];
                }
                else 
                {
                    ingredient = recipeItems[k + l * recipeWidth];
                }
            }

            if (!(ingredient == craftingInventory->getStackInRowAndColumn(i, j)))
            {
                return false;
            }
        }
    }

    return true;
}

std::vector<Ingredient> deserializeIngredients(std::vector<std::string> &pattern,
    std::unordered_map<std::string, Ingredient> &keys, int32_t patternWidth, int32_t patternHeight)
{
    std::vector<Ingredient> nonnulllist(patternWidth * patternHeight, Ingredient::EMPTY);

    std::unordered_set<std::string> set;
    Util::KeySet(set,keys);

    set.erase(std::remove_if(set.begin(), set.end(), [](const std::string& str){return str == " "; }), set.end());

    for(auto i = 0; i < pattern.size(); ++i) 
    {
        for(auto j = 0; j < pattern[i].size(); ++j) 
        {
            std::string s = pattern[i].substr(j, j + 1);
            Ingredient ingredient = keys[s];
            if (ingredient == nullptr) 
            {
                throw std::logic_error("Pattern references symbol '" + s + "' but it's not defined in the key");
            }

            set.erase(std::remove_if(set.begin(), set.end(), [&](const std::string& str){return str == s; }), set.end());

            nonnulllist[j + patternWidth * i] = ingredient;
        }
    }

    if (!set.empty()) 
    {
        throw std::logic_error("Key defines symbols that aren't used in pattern: " + set);
    }
    else 
    {
        return nonnulllist;
    }
}

std::vector<std::string> patternFromJson(const json &jsonArr)
{
    std::vector<std::string> astring (jsonArr.size());
    if (astring.size() > 3) 
    {
        throw std::logic_error("Invalid pattern: too many rows, 3 is maximum");
    }
    else if (astring.size() == 0) 
    {
        throw std::logic_error("Invalid pattern: empty pattern not allowed");
    }
    else 
    {
        for(auto i = 0; i < astring.size(); ++i) 
        {
            std::string s;
            auto key = fmt::format("pattern[{}]",i);
            jsonArr.at(i).at(key).get_to(s);

            if (s.size() > 3) 
            {
                throw std::logic_error("Invalid pattern: too many columns, 3 is maximum");
            }

            if (i > 0 && astring[0].length() != s.size()) 
            {
                throw std::logic_error("Invalid pattern: each row must be the same width");
            }

            astring[i] = s;
        }

        return astring;
    }
}

std::unordered_map<std::string, Ingredient> deserializeKey(const json &json)
{
    std::unordered_map<std::string, Ingredient> map;
    
    for(auto& entry : json.items())
    {
        if ((entry.key()).size() != 1) 
        {
            throw std::logic_error("Invalid key entry: '" + entry.key() + "' is an invalid symbol (must be 1 character only).");
        }

        if (" " == (entry.key())) 
        {
            throw std::logic_error("Invalid key entry: ' ' is a reserved symbol.");
        }

        map.emplace(entry.key(), deserializeIngredient(entry.value()));
    }

    map.emplace(" ", Ingredient::EMPTY);
    return map;
}

Ingredient deserializeIngredient(const json &jsonElement)
{
    if (jsonElement != nullptr && !jsonElement.isJsonNull()) 
    {
        if (jsonElement.is_object()) 
        {
            return Ingredient::fromStacks(deserializeItem(jsonElement, false));
        }
        else if (!jsonElement.is_array()) 
        {
            throw std::logic_error("Expected item to be object or array of objects");
        }
        else 
        {
            auto jsonarray = jsonElement;
            if (jsonarray.size() == 0) {
                throw std::logic_error("Item array cannot be empty, at least one item must be defined");
            }
            else 
            {
                std::vector<ItemStack> aitemstack(jsonarray.size());

                for(int i = 0; i < jsonarray.size(); ++i) 
                {
                    aitemstack[i] = deserializeItem(jsonarray.at(i), "item", false);
                }

                return Ingredient::fromStacks(aitemstack);
            }
        }
    }
    else 
    {
        throw std::logic_error("Item cannot be null");
    }
}

ItemStack deserializeItem(const json &json, bool useCount)
{
    std::string s;
    json.at("item").get_to(s);

    Item* item = Item::REGISTRY.getObject(ResourceLocation(s));
    if (item == nullptr) 
    {
        throw std::logic_error("Unknown item '" + s + "'");
    }
    else if (item->getHasSubtypes() && !json.contains("data")) 
    {
        throw std::logic_error("Missing data for item '" + s + "'");
    }
    else 
    {
        int i = 0;
        json.at("data").get_to(i);
        int j = 0;
        json.at("count").get_to(j);
        j = useCount ? j : 1;
        return ItemStack(item, j, i);
    }
}

std::vector<std::string> shrink(const std::vector<std::string> &toShrink)
{
    int32_t i = std::numeric_limits<int32_t>::max();
    int32_t j = 0;
    int32_t k = 0;
    int32_t l = 0;

    for(int32_t i1 = 0; i1 < toShrink.size(); ++i1) 
    {
        std::string s = toShrink[i1];
        i = MathHelper::min(i, firstNonSpace(s));
        int32_t j1 = lastNonSpace(s);
        j = MathHelper::max(j, j1);
        if (j1 < 0) 
        {
            if (k == i1) 
            {
                ++k;
            }

            ++l;
        }
        else 
        {
            l = 0;
        }
    }

    if (toShrink.size() == l) 
    {
        return std::vector<std::string>(1,"");
    }
    else 
    {
        std::vector<std::string> astring(toShrink.size() - l - k);

        for(int k1 = 0; k1 < astring.size(); ++k1) 
        {
            astring[k1] = toShrink[k1 + k].substr(i, j + 1);
        }

        return astring;
    }
}

constexpr int32_t firstNonSpace(std::string_view str)
{
    int32_t i = 0;
    for(i = 0; i < str.size() && str[i] == ' '; ++i) 
    {
    }

    return i;
}

constexpr int32_t lastNonSpace(std::string_view str)
{
    int32_t i = 0;
    for(i = str.length() - 1; i >= 0 && str[i] == ' '; --i) 
    {
    }

    return i;
}
