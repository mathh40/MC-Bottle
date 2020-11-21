#pragma once
#include <vector>
class Item;

class EnumEnchantmentType {
    public:

    enum class Type {
        ALL,
        ARMOR,
        ARMOR_FEET,
        ARMOR_LEGS,
        ARMOR_CHEST,
        ARMOR_HEAD,
        WEAPON,
        DIGGER,
        FISHING_ROD,
        BREAKABLE,
        BOW,
        WEARABLE
    };

    static EnumEnchantmentType ALL;
    static EnumEnchantmentType ARMOR;
    static EnumEnchantmentType ARMOR_FEET;
    static EnumEnchantmentType ARMOR_LEGS;
    static EnumEnchantmentType ARMOR_CHEST;
    static EnumEnchantmentType ARMOR_HEAD;
    static EnumEnchantmentType WEAPON;
    static EnumEnchantmentType DIGGER;
    static EnumEnchantmentType FISHING_ROD;
    static EnumEnchantmentType BREAKABLE;
    static EnumEnchantmentType BOW;
    static EnumEnchantmentType WEARABLE;

    friend bool operator==(const EnumEnchantmentType& lhs , const EnumEnchantmentType& rhs);
    friend bool operator!=(const EnumEnchantmentType& lhs , const EnumEnchantmentType& rhs);

    explicit EnumEnchantmentType(Type type);
        bool canEnchantItem(Item* itemIn);
        static std::vector<EnumEnchantmentType*> value();
    private:
        static std::vector<EnumEnchantmentType*> val;
        Type type;
    };
